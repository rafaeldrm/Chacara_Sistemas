//====================================================================================
//                   DEPENDÊNCIAS
//====================================================================================

#include "Autenticacao_LoRa.h"
#include "DEBUG_MODE.h"

//====================================================================================
//                   SETUP
//====================================================================================

void Autenticacao_LoRa_Inicializa()
{
        debugSetupDivisao(COR::AMARELO, "AUTENTICACAO LORA INICIALIZANDO", COR::MAGENTA, "=", 0);
}

//====================================================================================
//                   VARIÁVEIS INTERNAS
//====================================================================================

namespace LORA
{
namespace SEGURANCA
{

    namespace
    {
        // Folga que o Drimel soma à maior sequência já vista ao abrir uma sessão nova.
        constexpr uint32_t MARGEM_BOOT = 1000;
    }

//====================================================================================
//                   INICIALIZAR
//====================================================================================

    void Autenticacao_LoRa::Inicializar(const Chave& chave_do_sistema)
    {
        chave = chave_do_sistema;
        sequencia.Resetar();
        estado.Resetar();

        nonce_boot = 0;
        possui_boot_aceito = false;
        ultimo_nonce_boot = 0;
        ultima_base_boot = 0;
    }

//====================================================================================
//                   BUFFER DO MAC DO BOOT (PACOTE + NONCE)
//====================================================================================

    void Autenticacao_LoRa::MontarBufferBoot(const Pacote& p, uint32_t nonce, uint8_t buffer[TAMANHO_ASSINADO + 4]) const
    {
        memcpy(buffer, &p, TAMANHO_ASSINADO);

        for (uint8_t i = 0; i < 4; ++i)
            buffer[TAMANHO_ASSINADO + i] = (uint8_t)(nonce >> (8 * i));
    }

//====================================================================================
//                   BOOT: LADO DO SISTEMA
//====================================================================================

    void Autenticacao_LoRa::MontarPedidoBoot(Pacote& pedido, SISTEMA destino, SISTEMA remetente, uint32_t nonce)
    {
        sequencia.Resetar();
        nonce_boot = nonce;

        pedido = CriarPacote(destino, remetente, (uint8_t)ESPECIAL::CODIGO::BOOT, 0, nonce);
        mac.Gerar((const uint8_t*)&pedido, TAMANHO_ASSINADO, chave, pedido.mac);

        estado.IniciarSincronizacao();
    }



    AUTENTICACAO_RESULTADO Autenticacao_LoRa::ReceberRespostaBoot(const Pacote& resposta)
    {
        if (estado.Obter() != ESTADO_COMUNICACAO::SINCRONIZANDO)
            return AUTENTICACAO_RESULTADO::BOOT_INVALIDO;

        if (resposta.codigo != (uint8_t)ESPECIAL::CODIGO::BOOT_RESPOSTA)
            return AUTENTICACAO_RESULTADO::BOOT_INVALIDO;

        uint8_t buffer[TAMANHO_ASSINADO + 4];
        MontarBufferBoot(resposta, nonce_boot, buffer);

        if (!mac.Validar(buffer, sizeof(buffer), chave, resposta.mac))
            return AUTENTICACAO_RESULTADO::MAC_INVALIDO;

        sequencia.Sincronizar(resposta.getSequencia());
        estado.TornarOperacional();

        return AUTENTICACAO_RESULTADO::BOOT_ACEITO;
    }

//====================================================================================
//                   BOOT: LADO DO DRIMEL SYSTEM
//====================================================================================

    void Autenticacao_LoRa::IniciarBoot()
    {
        estado.IniciarBoot();
    }



    AUTENTICACAO_RESULTADO Autenticacao_LoRa::ReceberPedidoBoot(const Pacote& pedido, Pacote& resposta)
    {
        if (pedido.codigo != (uint8_t)ESPECIAL::CODIGO::BOOT)
            return AUTENTICACAO_RESULTADO::BOOT_INVALIDO;

        if (!mac.Validar((const uint8_t*)&pedido, TAMANHO_ASSINADO, chave, pedido.mac))
            return AUTENTICACAO_RESULTADO::MAC_INVALIDO;

        const uint32_t nonce = pedido.getSequencia();

        // Pedido repetido (resposta perdida e o sistema pediu de novo):
        // repete a mesma base, sem abrir outra sessão.
        if (!possui_boot_aceito || nonce != ultimo_nonce_boot)
        {
            const uint32_t saida = sequencia.UltimaSaida();
            const uint32_t entrada = sequencia.UltimaRecebida();
            const uint32_t maior = (saida > entrada) ? saida : entrada;

            ultima_base_boot = maior + MARGEM_BOOT;
            ultimo_nonce_boot = nonce;
            possui_boot_aceito = true;

            sequencia.Sincronizar(ultima_base_boot);
            estado.TornarOperacional();
        }

        resposta = CriarPacote(
            (SISTEMA)ObterRemetente(pedido.sistema),
            (SISTEMA)ObterDestino(pedido.sistema),
            (uint8_t)ESPECIAL::CODIGO::BOOT_RESPOSTA,
            0,
            ultima_base_boot);

        uint8_t buffer[TAMANHO_ASSINADO + 4];
        MontarBufferBoot(resposta, nonce, buffer);
        mac.Gerar(buffer, sizeof(buffer), chave, resposta.mac);

        return AUTENTICACAO_RESULTADO::BOOT_ACEITO;
    }

//====================================================================================
//                   ESTADO
//====================================================================================

    bool Autenticacao_LoRa::EstaOperacional() const
    {
        return estado.PodeTransmitirNormalmente();
    }



    ESTADO_COMUNICACAO Autenticacao_LoRa::ObterEstado() const
    {
        return estado.Obter();
    }

//====================================================================================
//                   ASSINAR
//====================================================================================

    bool Autenticacao_LoRa::Assinar(Pacote& p)
    {
        if (!estado.PodeTransmitirNormalmente())
            return false;

        p.setSequencia(sequencia.ProximaSaida());

        return mac.Gerar((const uint8_t*)&p, TAMANHO_ASSINADO, chave, p.mac);
    }

//====================================================================================
//                   VALIDAR
//====================================================================================

    AUTENTICACAO_RESULTADO Autenticacao_LoRa::Validar(const Pacote& p, RegistroAnalise* registro)
    {
        if (!estado.PodeTransmitirNormalmente())
            return AUTENTICACAO_RESULTADO::NAO_SINCRONIZADO;

        if (!mac.Validar((const uint8_t*)&p, TAMANHO_ASSINADO, chave, p.mac))
            return AUTENTICACAO_RESULTADO::MAC_INVALIDO;

        const SEQUENCIA_RESULTADO resultado = sequencia.ClassificarRecebida(p.getSequencia());

        if (registro != nullptr)
            *registro = analise.Classificar(resultado, p.getSequencia());

        switch (resultado)
        {
        case SEQUENCIA_RESULTADO::NOVA_MENSAGEM:     return AUTENTICACAO_RESULTADO::NOVA_MENSAGEM;
        case SEQUENCIA_RESULTADO::REPETICAO_INTERNA: return AUTENTICACAO_RESULTADO::REPETICAO_INTERNA;
        default:                                    return AUTENTICACAO_RESULTADO::SEQUENCIA_INFERIOR;
        }
    }

}
}