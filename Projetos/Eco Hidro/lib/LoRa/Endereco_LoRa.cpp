//====================================================================================
//                   DEPENDÊNCIAS
//====================================================================================

#include "Endereco_LoRa.h"
#include "DEBUG_MODE.h"

#include "LoRa_Transporte.h"
#include "Driver_LoRa.h"
#include "Chaves_LoRa.h"
#include "Chaves_LoRa.h"
#include "Autenticacao_LoRa.h"      // era: #include "Analise_LoRa.h"

//====================================================================================
//                   VARIÁVEIS INTERNAS
//====================================================================================

using LORA::AUTENTICACAO::ResultadoAutenticacao;

static LORA::AUTENTICACAO::Autenticacao_LoRa autenticacao;

static bool bootPendente = false;       // pedido de BOOT montado e ainda sem resposta
static LORA::Pacote pedidoBoot;         // guardado para reenviar com o MESMO nonce

//====================================================================================
//                   SETUP
//====================================================================================

void Endereco_LoRa_Inicializa()
{
    debugSetupDivisao(COR::AMARELO, "ENDERECO LORA INICIALIZANDO", COR::MAGENTA, "=", 45);

    debugVariavel("Endereço local: ", LORA::SistemaParaTexto((uint8_t)ENDERECO_LOCAL), NEGRITO, VERDE, AZUL);
    debugVariavel("Destino padrão: ", LORA::SistemaParaTexto((uint8_t)ENDERECO_DESTINO_PADRAO), NEGRITO, VERDE, AZUL);

    autenticacao.Inicializar(LORA::AUTENTICACAO::ObterChave());
}
//====================================================================================
//                   BOOT
//====================================================================================

bool Endereco_LoRa_PedirBoot()
{
    if (!bootPendente)
    {
        autenticacao.MontarPedidoBoot(pedidoBoot, ENDERECO_DESTINO_PADRAO, ENDERECO_LOCAL, Driver_LoRa_Aleatorio32());
        bootPendente = true;
    }

    debugAviso("Endereco_LoRa: enviando pedido de BOOT...");

    return LoRa_Enviar_Pacote(pedidoBoot);
}



bool Endereco_LoRa_Operacional()
{
    return autenticacao.EstaOperacional();
}

//====================================================================================
//                   ENVIO
//====================================================================================

bool Endereco_LoRa_Montar(uint8_t codigo, uint16_t valor, LORA::Pacote& pacote)
{
    pacote = LORA::CriarPacote(
        ENDERECO_DESTINO_PADRAO,    // DESTINO (configurado no .h)
        ENDERECO_LOCAL,             // ORIGEM (configurado no .h)
        codigo,
        valor
    );

    if (!autenticacao.Assinar(pacote))
    {
        debugErro("Endereco_LoRa: sem sessão autenticada (BOOT pendente)");
        return false;
    }

    return true;
}



void Endereco_LoRa_Mensagem(uint8_t codigo, uint16_t valor)
{
    debugAviso("Endereco_LoRa: Montando pacote para envio...");

    LORA::Pacote p;

    if (!Endereco_LoRa_Montar(codigo, valor, p))
        return;

    LoRa_Enviar_Pacote(p);
}

//====================================================================================
//                   RECEBIMENTO
//====================================================================================

bool Endereco_LoRa_Receber(LORA::Pacote& pacote, bool* repetida)
{
    if (repetida != nullptr)
        *repetida = false;

    LORA::Pacote recebido;

    if (!LoRa_Receber_Pacote(recebido))
        return false;

    // Só interessa o que é para mim e veio do Drimel
    if (LORA::ObterDestino(recebido.sistema) != (uint8_t)ENDERECO_LOCAL ||
        LORA::ObterRemetente(recebido.sistema) != (uint8_t)ENDERECO_DESTINO_PADRAO)
    {
        debugAviso("Endereco_LoRa: pacote de/para outro sistema, ignorado");
        return false;
    }

    // Resposta do BOOT: consumida aqui mesmo, as camadas de cima nem veem
    if (recebido.codigo == (uint8_t)LORA::ESPECIAL::CODIGO::BOOT_RESPOSTA)
    {
        if (autenticacao.ReceberRespostaBoot(recebido) == ResultadoAutenticacao::BOOT_ACEITO)
        {
            bootPendente = false;
            debugOk("Endereco_LoRa: BOOT aceito, sessão autenticada");
        }
        else
        {
            debugAviso("Endereco_LoRa: resposta de BOOT descartada");
        }

        return false;
    }

    // Mensagem normal: passa pela Autenticação
    switch (autenticacao.Validar(recebido))
    {
    case ResultadoAutenticacao::NOVA_MENSAGEM:
        pacote = recebido;
        debugOk("Endereco_LoRa: mensagem autenticada");
        return true;

    case ResultadoAutenticacao::REPETICAO_INTERNA:
        if (repetida != nullptr)
            *repetida = true;
        debugAviso("Endereco_LoRa: cópia repetida ignorada");
        return false;

    case ResultadoAutenticacao::MAC_INVALIDO:
        debugErro("Endereco_LoRa: MAC INVÁLIDO, pacote descartado");
        return false;

    case ResultadoAutenticacao::SEQUENCIA_INFERIOR:
        debugErro("Endereco_LoRa: sequência antiga, descartado");
        return false;

    case ResultadoAutenticacao::NAO_SINCRONIZADO:
        debugAviso("Endereco_LoRa: ainda sem sessão (aguardando BOOT), descartado");
        return false;

    default:
        return false;
    }
}