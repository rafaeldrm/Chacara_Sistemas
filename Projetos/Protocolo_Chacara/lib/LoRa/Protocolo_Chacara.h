#pragma once
#include <Arduino.h>

#include "DEBUG_MODE.h"

/*

/------------------------------------------------------------------------------------------------

 PROTOCOLO CHÁCARA - COMUNICAÇÃO BINÁRIA LORA v4.0

 FORMATO DO PACOTE (12 BYTES FIXO):
 [SISTEMA][CODIGO][VALOR_LOW][VALOR_HIGH][SEQ 4 bytes][MAC 4 bytes]

 BYTE 1 (SISTEMA):
 [DESTINO (4 bits)] [REMETENTE (4 bits)]

 Exemplo:
 RESIDENCIAL (DRIMEL_SYSTEM) -> SOLAR (SOLARIUS24)
 DESTINO = 2
 REMETENTE = 1
 BYTE = 0x21

 SOLAR (SOLARIUS24) -> RESIDENCIAL (DRIMEL_SYSTEM)
 DESTINO = 1
 REMETENTE = 2
 BYTE = 0x12
*/

//====================================================================================
//                   CONSTANTES
//====================================================================================

namespace LORA
{

    //constexpr uint8_t CHAVE_SEGURANCA = 0x5A;
    constexpr uint16_t VALOR_SEM_DADOS = 0;

    constexpr uint8_t TAMANHO_MAC_LORA = 4;        // SipHash-2-4 truncado
    constexpr uint8_t CODIGO_ESPECIAL_MINIMO = 200; // 200+ = códigos comuns a todos os sistemas

    //====================================================================================
    //                   SISTEMAS (0 a 15)
    //====================================================================================

    enum class SISTEMA : uint8_t
    {
        BROADCAST = 0,
        DRIMEL_SYSTEM = 1,
        SOLARIUS24 = 2,
        ECO_HIDRO = 3,
        GATE_DRIMEL = 4
    };

    //====================================================================================
    //                   MONTAGEM BYTE SISTEMA
    //====================================================================================

    inline uint8_t MontarSistema(uint8_t destino, uint8_t remetente)
    {
        return ((destino & 0x0F) << 4) | (remetente & 0x0F);
    }

    inline uint8_t ObterDestino(uint8_t sistema)
    {
        return (sistema >> 4) & 0x0F;
    }

    inline uint8_t ObterRemetente(uint8_t sistema)
    {
        return sistema & 0x0F;
    }

    //====================================================================================
    //                   CÓDIGOS POR SISTEMA
    //====================================================================================

    //====================================================================================
    //@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
    //====================================================================================
    //                   SISTEMA SOLARIUS24 TELEMETRIA
    //====================================================================================
    //@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
    //====================================================================================

    namespace SOLARIUS24
    {

        enum class CODIGO : uint8_t
        {

            VOLTAGEM_SISTEMA = 1,
            VARIACAO_BATERIAS = 2,
            CORRENTE_MAQUINA = 3,
            TEMPERATURA_MPPT = 4,

            REDE_ATUAL = 5,
            REDE_MAXIMA = 6,
            REDE_MINIMA = 7,

            STATUS_FONTE_USINA = 8,
            STATUS_REDE = 9,

            ESTADO_CALAMIDADE = 10,
            ESTADO_CARGA = 11,
            ESTADO_REDE = 12,
            ESTADO_URGENCIA = 13,

            //---------------------ordens do Sistema (DRIMEL_SYSTEM) ----------------------------------//

            CMD_LIGAR_FONTE_USINA = 100,
            CMD_STATUS_GERAL = 101,
            CMD_LORA_PING = 102,

            //-------------------- SOLARIUS24 para o DRIMEL_SYSTEM -----------------------------------------------//

            NENHUM = 200,
            ACK_STATUS_GERAL_FIM = 210,

            SISTEMA_BOOT = 250
        };
    }

    //====================================================================================
    //@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
    //====================================================================================
    //====================================================================================
    //                   SISTEMA ECO HIDRO TELEMETRIA
    //====================================================================================
    //====================================================================================
    //@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
    //====================================================================================
    namespace ECO_HIDRO
    {

        enum class CODIGO : uint8_t
        {
            NOTIFICACAO_MAPA_BOIAS_COMPLETO = 2,
            NOTIFICACAO_STATUS_BOMBA_CASA = 3,
            NOTIFICACAO_STATUS_BOMBA_RESERVATORIO = 4,
            NOTIFICACAO_ENVIO_SENHA_PORTAO = 5,
            NOTIFICACAO_PROTECAO_BOMBA_CASA = 6,
            NOTIFICACAO_TEMPERATURA_BOMBA = 7,
            NOTIFICACAO_BOMBA_TRAVADA = 8,

            //--------ordens do sistema DRIMEL_SYSTEM -------------------------------------//

            ORDEM_CICLO_COMPLETO = 101,
            ORDEM_MEIO_CICLO = 102,
            ORDEM_DESLIGAR_BOMBA_CASA = 103,
            ORDEM_DESLIGAR_BOMBA_RESERVATORIO = 104,
            ORDEM_BLOQUEADO = 105,
            ORDEM_NORMAL = 106,
            ORDEM_SENHA_PROTECAO_PORTAO = 107,
            ORDEM_PEDIDO_VALOR_VOLTAGEM = 108,
            ORDEM_DESATIVA_SENSOR_IR = 109,

            //--------------Resposta de Comandos para Drimel System -------------------------//

            RESPOSTA_ORDEM_CICLO_COMPLETO = 150,
            RESPOSTA_ORDEM_MEIO_CICLO = 151,
            RESPOSTA_ORDEM_BLOQUEADO = 152,
            RESPOSTA_VOLTAGEM_VALOR = 153,
        };
    }

    //====================================================================================
    //@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
    //====================================================================================
    //====================================================================================
    //                   SISTEMA GATE_DRIMEL TELEMETRIA
    //====================================================================================
    //====================================================================================
    //@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
    //====================================================================================

    namespace GATE_DRIMEL
    {

        enum class CODIGO : uint8_t
        {

            STATUS_POSICAO = 1,
            SENSOR_PRESENCA = 2,
            TEMPERATURA = 3,
            UMIDADE = 4,

            //------------- ordens do Sistema DRIMEL_SYSTEM ------------------------------

            CMD_ABRIR = 100,
            CMD_FECHAR = 101,
            CMD_SENHA = 102,
            CMD_STATUS_GERAL = 103,
            CMD_LORA_PING = 104,
            CMD_BLOQUEADO = 105,
            CMD_NORMAL = 106,
            CMD_FERIAS = 107,
            CMD_FESTA = 108,

            //-------------------- GATE_DRIMEL para o DRIMEL_SYSTEM -----------------------------------------------//

            NENHUM = 200,
            ACK_STATUS_GERAL_FIM = 210,

            SISTEMA_BOOT = 250
        };
    }

//====================================================================================
//                   ESPECIAL
//====================================================================================

    namespace ESPECIAL
{
    enum class CODIGO : uint8_t
    {
        BOOT   = 200,   // sistema -> Drimel (o campo seq leva o nonce)
        BOOT_RESPOSTA = 201,   // Drimel -> sistema (o campo seq leva a sequência inicial)
        SINAL_PING    = 202,
        SINAL_PONG    = 203
    };
}

inline const char *EspecialParaTexto(uint8_t codigo)
{
    switch ((ESPECIAL::CODIGO)codigo)
    {
    case ESPECIAL::CODIGO::BOOT:            return "BOOT";
    case ESPECIAL::CODIGO::BOOT_RESPOSTA:   return "BOOT_RESPOSTA";
    case ESPECIAL::CODIGO::SINAL_PING:      return "SINAL_PING";
    case ESPECIAL::CODIGO::SINAL_PONG:      return "SINAL_PONG";
    default:                                return nullptr;
    }
}

    //====================================================================================
    //                   STRUCT PACOTE
    //====================================================================================

    struct __attribute__((packed)) Pacote
{
    uint8_t sistema;   // [DESTINO][REMETENTE]
    uint8_t codigo;
    uint8_t valor_low;
    uint8_t valor_high;
    uint8_t seq[4];                  // little-endian, igual nos dois lados
    uint8_t mac[TAMANHO_MAC_LORA];

    void setValor(uint16_t valor)
    {
        valor_low = valor & 0xFF;
        valor_high = (valor >> 8) & 0xFF;
    }

    uint16_t getValor() const
    {
        return (uint16_t(valor_high) << 8) | valor_low;
    }

    void setSequencia(uint32_t s)
    {
        seq[0] = s & 0xFF;
        seq[1] = (s >> 8) & 0xFF;
        seq[2] = (s >> 16) & 0xFF;
        seq[3] = (s >> 24) & 0xFF;
    }

    uint32_t getSequencia() const
    {
        return (uint32_t(seq[3]) << 24) | (uint32_t(seq[2]) << 16) |
               (uint32_t(seq[1]) << 8)  |  uint32_t(seq[0]);
    }
};

static_assert(sizeof(Pacote) == 12, "Pacote deve ter 12 bytes");

// Quantidade de bytes que o MAC cobre: tudo antes do campo mac
constexpr uint8_t TAMANHO_ASSINADO = sizeof(Pacote) - TAMANHO_MAC_LORA;

    //====================================================================================
    //                   CRIAÇÃO DE PACOTE
    //====================================================================================

    inline Pacote CriarPacote(SISTEMA destino, SISTEMA remetente, uint8_t codigo,
                          uint16_t valor = 0, uint32_t sequencia = 0)
{
    Pacote p;
    memset(&p, 0, sizeof(p));   // mac fica zerado; quem assina é a camada de Autenticação

    p.sistema = MontarSistema((uint8_t)destino, (uint8_t)remetente);
    p.codigo = codigo;
    p.setValor(valor);
    p.setSequencia(sequencia);

    return p;
}
    //====================================================================================
    //                   DESTINO
    //====================================================================================

    inline bool EhParaMim(uint8_t sistema, SISTEMA meuID)
    {
        uint8_t destino = ObterDestino(sistema);

        return (destino == (uint8_t)meuID || destino == (uint8_t)SISTEMA::BROADCAST);
    }

    //====================================================================================
    //                   TEXTO (DEBUG)
    //====================================================================================

    inline const char *SistemaParaTexto(uint8_t id)
    {
        switch ((SISTEMA)id)
        {
        case SISTEMA::BROADCAST:                return "BROADCAST";
        case SISTEMA::DRIMEL_SYSTEM:            return "DRIMEL_SYSTEM";
        case SISTEMA::SOLARIUS24:               return "SOLARIUS24";
        case SISTEMA::ECO_HIDRO:                return "ECO_HIDRO";
        case SISTEMA::GATE_DRIMEL:              return "GATE_DRIMEL";

        default:                                return "INVALIDO";
        }
    }

    inline const char *CodigoParaTexto(uint8_t sistema, uint8_t codigo)
    {
        switch ((SISTEMA)sistema)
        {

            //====================================================================================
            //@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
            //====================================================================================
            //====================================================================================
            //                   SOLARIUS24
            //====================================================================================
            //====================================================================================
            //@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
            //====================================================================================

        case SISTEMA::SOLARIUS24:
            switch ((SOLARIUS24::CODIGO)codigo)
            {
            case SOLARIUS24::CODIGO::VOLTAGEM_SISTEMA:
                return "VOLTAGEM_SISTEMA";
            case SOLARIUS24::CODIGO::VARIACAO_BATERIAS:
                return "VARIACAO_BATERIAS";
            case SOLARIUS24::CODIGO::CORRENTE_MAQUINA:
                return "CORRENTE_MAQUINA";
            case SOLARIUS24::CODIGO::TEMPERATURA_MPPT:
                return "TEMPERATURA_MPPT";
            case SOLARIUS24::CODIGO::REDE_ATUAL:
                return "REDE_ATUAL";
            case SOLARIUS24::CODIGO::REDE_MAXIMA:
                return "REDE_MAXIMA";
            case SOLARIUS24::CODIGO::REDE_MINIMA:
                return "REDE_MINIMA";
            case SOLARIUS24::CODIGO::STATUS_FONTE_USINA:
                return "STATUS_FONTE_USINA";
            case SOLARIUS24::CODIGO::STATUS_REDE:
                return "STATUS_REDE";
            case SOLARIUS24::CODIGO::ESTADO_CALAMIDADE:
                return "ESTADO_CALAMIDADE";
            case SOLARIUS24::CODIGO::ESTADO_CARGA:
                return "ESTADO_CARGA";
            case SOLARIUS24::CODIGO::ESTADO_REDE:
                return "ESTADO_REDE";
            case SOLARIUS24::CODIGO::ESTADO_URGENCIA:
                return "ESTADO_URGENCIA";
            case SOLARIUS24::CODIGO::CMD_LIGAR_FONTE_USINA:
                return "CMD_LIGAR_FONTE_USINA";
            case SOLARIUS24::CODIGO::CMD_STATUS_GERAL:
                return "CMD_STATUS_GERAL";
            case SOLARIUS24::CODIGO::CMD_LORA_PING:
                return "CMD_LORA_PING";
            case SOLARIUS24::CODIGO::NENHUM:
                return "NENHUM";
            case SOLARIUS24::CODIGO::ACK_STATUS_GERAL_FIM:
                return "ACK_STATUS_GERAL_FIM";
            case SOLARIUS24::CODIGO::SISTEMA_BOOT:
                return "SISTEMA_BOOT";

            default:
                return "SOLARIUS24_DESCONHECIDO";
            }

            //====================================================================================
            //@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
            //====================================================================================
            //====================================================================================
            //                   ECO HIDRO
            //====================================================================================
            //====================================================================================
            //@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
            //====================================================================================

        case SISTEMA::ECO_HIDRO:
            switch ((ECO_HIDRO::CODIGO)codigo)
            {
            case ECO_HIDRO::CODIGO::NOTIFICACAO_MAPA_BOIAS_COMPLETO:                return "NOTIFICACAO_MAPA_BOIAS_COMPLETO";
            case ECO_HIDRO::CODIGO::NOTIFICACAO_STATUS_BOMBA_CASA:                  return "NOTIFICACAO_STATUS_BOMBA_CASA";
            case ECO_HIDRO::CODIGO::NOTIFICACAO_STATUS_BOMBA_RESERVATORIO:          return "NOTIFICACAO_STATUS_BOMBA_RESERVATORIO";
            case ECO_HIDRO::CODIGO::NOTIFICACAO_ENVIO_SENHA_PORTAO:                 return "NOTIFICACAO_ENVIO_SENHA_PORTAO";
            case ECO_HIDRO::CODIGO::NOTIFICACAO_PROTECAO_BOMBA_CASA:                return "NOTIFICACAO_PROTECAO_BOMBA_CASA";
            case ECO_HIDRO::CODIGO::NOTIFICACAO_TEMPERATURA_BOMBA:                  return "NOTIFICACAO_TEMPERATURA_BOMBA";
            case ECO_HIDRO::CODIGO::NOTIFICACAO_BOMBA_TRAVADA:                      return "NOTIFICACAO_BOMBA_TRAVADA";

            case ECO_HIDRO::CODIGO::ORDEM_CICLO_COMPLETO:                           return "ORDEM_CICLO_COMPLETO";
            case ECO_HIDRO::CODIGO::ORDEM_MEIO_CICLO:                               return "ORDEM_MEIO_CICLO";
            case ECO_HIDRO::CODIGO::ORDEM_DESLIGAR_BOMBA_CASA:                      return "ORDEM_DESLIGAR_BOMBA_CASA";
            case ECO_HIDRO::CODIGO::ORDEM_DESLIGAR_BOMBA_RESERVATORIO:              return "ORDEM_DESLIGAR_BOMBA_RESERVATORIO";
            case ECO_HIDRO::CODIGO::ORDEM_BLOQUEADO:                                return "ORDEM_BLOQUEADO";
            case ECO_HIDRO::CODIGO::ORDEM_NORMAL:                                   return "ORDEM_NORMAL";
            case ECO_HIDRO::CODIGO::ORDEM_SENHA_PROTECAO_PORTAO:                    return "ORDEM_SENHA_PROTECAO_PORTAO";
            case ECO_HIDRO::CODIGO::ORDEM_PEDIDO_VALOR_VOLTAGEM:                    return "ORDEM_PEDIDO_VALOR_VOLTAGEM";
            case ECO_HIDRO::CODIGO::ORDEM_DESATIVA_SENSOR_IR:                       return "ORDEM_DESATIVA_SENSOR_IR";

            case ECO_HIDRO::CODIGO::RESPOSTA_ORDEM_CICLO_COMPLETO:                  return "RESPOSTA_ORDEM_CICLO_COMPLETO";
            case ECO_HIDRO::CODIGO::RESPOSTA_ORDEM_MEIO_CICLO:                      return "RESPOSTA_ORDEM_MEIO_CICLO";
            case ECO_HIDRO::CODIGO::RESPOSTA_ORDEM_BLOQUEADO:                       return "RESPOSTA_ORDEM_BLOQUEADO";
            case ECO_HIDRO::CODIGO::RESPOSTA_VOLTAGEM_VALOR:                        return "RESPOSTA_VOLTAGEM_VALOR";

            default:
                return "ECO_HIDRO_DESCONHECIDO";
            }

            //====================================================================================
            //@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
            //====================================================================================
            //====================================================================================
            //                   GATE DRIMEL
            //====================================================================================
            //====================================================================================
            //@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
            //====================================================================================

        case SISTEMA::GATE_DRIMEL:
            switch ((GATE_DRIMEL::CODIGO)codigo)
            {
            case GATE_DRIMEL::CODIGO::STATUS_POSICAO:
                return "STATUS_POSICAO";
            case GATE_DRIMEL::CODIGO::SENSOR_PRESENCA:
                return "SENSOR_PRESENCA";
            case GATE_DRIMEL::CODIGO::TEMPERATURA:
                return "TEMPERATURA";
            case GATE_DRIMEL::CODIGO::UMIDADE:
                return "UMIDADE";
            case GATE_DRIMEL::CODIGO::CMD_ABRIR:
                return "CMD_ABRIR";
            case GATE_DRIMEL::CODIGO::CMD_FECHAR:
                return "CMD_FECHAR";
            case GATE_DRIMEL::CODIGO::CMD_SENHA:
                return "CMD_SENHA";
            case GATE_DRIMEL::CODIGO::CMD_STATUS_GERAL:
                return "CMD_STATUS_GERAL";
            case GATE_DRIMEL::CODIGO::CMD_LORA_PING:
                return "CMD_LORA_PING";
            case GATE_DRIMEL::CODIGO::CMD_BLOQUEADO:
                return "CMD_BLOQUEADO";
            case GATE_DRIMEL::CODIGO::CMD_NORMAL:
                return "CMD_NORMAL";
            case GATE_DRIMEL::CODIGO::CMD_FERIAS:
                return "CMD_FERIAS";
            case GATE_DRIMEL::CODIGO::CMD_FESTA:
                return "CMD_FESTA";
            case GATE_DRIMEL::CODIGO::NENHUM:
                return "NENHUM";
            case GATE_DRIMEL::CODIGO::ACK_STATUS_GERAL_FIM:
                return "ACK_STATUS_GERAL_FIM";
            case GATE_DRIMEL::CODIGO::SISTEMA_BOOT:
                return "SISTEMA_BOOT";

            default:
                return "GATE_DRIMEL_DESCONHECIDO";
            }

        default:
            return "SISTEMA_INVALIDO";
        }
    }

    //====================================================================================
    //                   DEBUG_PACOTE
    //====================================================================================

   inline void PrintPacote(const Pacote &p)
{
    uint8_t destino = ObterDestino(p.sistema);
    uint8_t remetente = ObterRemetente(p.sistema);

    uint8_t sistema_dono_codigo = (remetente == (uint8_t)SISTEMA::DRIMEL_SYSTEM) ? destino : remetente;

    const char *nome = EspecialParaTexto(p.codigo);
    if (nome == nullptr)
        nome = CodigoParaTexto(sistema_dono_codigo, p.codigo);

    debugVariavel("[", SistemaParaTexto(remetente), NEGRITO, AMARELO, LARANJA);
    debugVariavel(" -> ", SistemaParaTexto(destino), NEGRITO, AMARELO, LARANJA);
    debugVariavel("] ", nome, NEGRITO, AMARELO, LARANJA);
    debugVariavel(" = ", p.getValor(), NEGRITO, AMARELO, LARANJA);
    debugVariavel(" seq ", p.getSequencia(), NEGRITO, AMARELO, LARANJA);
}

}