#pragma once
#include <Arduino.h>

#include "DEBUG_MODE.h"

/* 

/------------------------------------------------------------------------------------------------                        

 PROTOCOLO CHÁCARA - COMUNICAÇÃO BINÁRIA LORA v3.0

 FORMATO DO PACOTE (5 BYTES FIXO):
 [SISTEMA] [CODIGO] [VALOR_LOW] [VALOR_HIGH] [HASH]

 BYTE 1 (SISTEMA):
 [DESTINO (4 bits)] [REMETENTE (4 bits)]

 Exemplo:
 RESIDENCIAL (DRIMEL_SYSTEM) -> SOLAR (SOLARIUS24)
 DESTINO = 2
 REMETENTE = 1
 BYTE = 0x21

 SOLAR (SOLARIUS24) -> RESIDENCIAL (DRIMEL_SYSTEM)
 DESTINO = 1
 REMETENTE = 2a
 BYTE = 0x12
*/

//====================================================================================
//                   CONSTANTES
//====================================================================================

namespace LORA {

constexpr uint8_t CHAVE_SEGURANCA = 0x5A;
constexpr uint16_t VALOR_SEM_DADOS = 0;

//====================================================================================
//                   SISTEMAS (0 a 15)
//====================================================================================

enum class SISTEMA : uint8_t {
    BROADCAST     = 0,
    DRIMEL_SYSTEM    = 1,
    SOLARIUS24    = 2,
    ECO_HIDRO     = 3,
    GATE_DRIMEL   = 4
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

namespace SOLARIUS24 {

    enum class CODIGO : uint8_t {

        VOLTAGEM_SISTEMA        = 1,
        VARIACAO_BATERIAS       = 2,
        CORRENTE_MAQUINA        = 3,
        TEMPERATURA_MPPT        = 4,

        REDE_ATUAL              = 5,
        REDE_MAXIMA             = 6,
        REDE_MINIMA             = 7,

        STATUS_FONTE_USINA      = 8,
        STATUS_REDE             = 9,

        ESTADO_CALAMIDADE       = 10,
        ESTADO_CARGA            = 11,
        ESTADO_REDE             = 12,
        ESTADO_URGENCIA         = 13,

//---------------------ordens do Sistema (DRIMEL_SYSTEM) ----------------------------------//

        CMD_LIGAR_FONTE_USINA   = 100,
        CMD_STATUS_GERAL        = 101,
        CMD_LORA_PING           = 102,

//-------------------- SOLARIUS24 para o DRIMEL_SYSTEM -----------------------------------------------//

        ACK_STATUS_GERAL_FIM    = 210,

        SISTEMA_BOOT            = 250
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
namespace ECO_HIDRO {

    enum class CODIGO : uint8_t {

        NIVEL_BOIAS                     = 1,
        STATUS_BOMBA_CASA               = 2,
        STATUS_BOMBA_RESERVATORIO       = 3,
        VOLTAGEM_SISTEMA                = 4,

    //--------ordens do sistema DRIMEL_SYSTEM -------------------------------------//

        CMD_LIGAR_BOMBA_CASA                = 100,
        CMD_DESLIGAR_BOMBA_CASA             = 101,
        CMD_DESLIGAR_BOMBA_RESERVATORIO     = 102,
        CMD_STATUS_GERAL                    = 103,
        CMD_LORA_PING                       = 104,
        CMD_BLOQUEADO                       = 105,
        CMD_NORMAL                          = 106,
        CMD_PORTAO_ABRIR                    = 107,
        CMD_SENHA_PROTECAO_PORTAO           = 108,
        CMD_CICLO_COMPLETO                  = 109,
        CMD_MEIO_CICLO                      = 110,


//-------------------- ECO_HIDRO para o DRIMEL_SYSTEM -----------------------------------------------//

        ACK_STATUS_GERAL_FIM    = 210,

        SISTEMA_BOOT            = 250
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

namespace GATE_DRIMEL {

    enum class CODIGO : uint8_t {

        STATUS_POSICAO      = 1,
        SENSOR_PRESENCA     = 2,
        TEMPERATURA         = 3,
        UMIDADE             = 4,

//------------- ordens do Sistema DRIMEL_SYSTEM ------------------------------

        CMD_ABRIR           = 100,
        CMD_FECHAR          = 101,
        CMD_SENHA           = 102,
        CMD_STATUS_GERAL    = 103,
        CMD_LORA_PING       = 104,
        CMD_BLOQUEADO       = 105,
        CMD_NORMAL          = 106,
        CMD_FERIAS          = 107,
        CMD_FESTA           = 108,

//-------------------- GATE_DRIMEL para o DRIMEL_SYSTEM -----------------------------------------------//

        ACK_STATUS_GERAL_FIM    = 210,

        SISTEMA_BOOT            = 250
    };
}

//====================================================================================
//                   STRUCT PACOTE
//====================================================================================

struct __attribute__((packed)) Pacote {

    uint8_t sistema;      // [DESTINO][REMETENTE]
    uint8_t codigo;
    uint8_t valor_low;
    uint8_t valor_high;
    uint8_t hash;

    void setValor(uint16_t valor)
    {
        valor_low  = valor & 0xFF;
        valor_high = (valor >> 8) & 0xFF;
    }

    uint16_t getValor() const
    {
        return (uint16_t(valor_high) << 8) | valor_low;
    }
};

//====================================================================================
//                   CRIAÇÃO DE PACOTE
//====================================================================================

inline Pacote CriarPacote(SISTEMA destino, SISTEMA remetente, uint8_t codigo, uint16_t valor = 0)
{
    Pacote p;

    p.sistema = MontarSistema((uint8_t)destino, (uint8_t)remetente);
    p.codigo  = codigo;

    p.setValor(valor);

    p.hash = p.sistema ^ p.codigo ^ p.valor_low ^ p.valor_high ^ CHAVE_SEGURANCA;

    return p;
}

//====================================================================================
//                   VALIDAÇÃO
//====================================================================================

inline bool ValidarPacote(const Pacote& p)
{
    uint8_t hash =
        p.sistema ^ p.codigo ^ p.valor_low ^ p.valor_high ^ CHAVE_SEGURANCA;

    return (hash == p.hash);
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

inline const char* SistemaParaTexto(uint8_t id)
{
    switch((SISTEMA)id) {
        case SISTEMA::DRIMEL_SYSTEM: return "DRIMEL_SYSTEM";
        case SISTEMA::SOLARIUS24: return "SOLARIUS24";
        case SISTEMA::ECO_HIDRO: return "ECO_HIDRO";
        case SISTEMA::GATE_DRIMEL: return "GATE_DRIMEL";
        default: return "INVALIDO";
    }
}

inline const char* CodigoParaTexto(uint8_t sistema, uint8_t codigo)
{
    switch((SISTEMA)sistema)
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
            switch((SOLARIUS24::CODIGO)codigo)
            {
                case SOLARIUS24::CODIGO::VOLTAGEM_SISTEMA: return "VOLTAGEM_SISTEMA";
                case SOLARIUS24::CODIGO::VARIACAO_BATERIAS: return "VARIACAO_BATERIAS";
                case SOLARIUS24::CODIGO::CORRENTE_MAQUINA: return "CORRENTE_MAQUINA";
                case SOLARIUS24::CODIGO::TEMPERATURA_MPPT: return "TEMPERATURA_MPPT";
                case SOLARIUS24::CODIGO::REDE_ATUAL: return "REDE_ATUAL";
                case SOLARIUS24::CODIGO::REDE_MAXIMA: return "REDE_MAXIMA";
                case SOLARIUS24::CODIGO::REDE_MINIMA: return "REDE_MINIMA";
                case SOLARIUS24::CODIGO::STATUS_FONTE_USINA: return "STATUS_FONTE_USINA";
                case SOLARIUS24::CODIGO::STATUS_REDE: return "STATUS_REDE";
                case SOLARIUS24::CODIGO::ESTADO_CALAMIDADE: return "ESTADO_CALAMIDADE";
                case SOLARIUS24::CODIGO::ESTADO_CARGA: return "ESTADO_CARGA";
                case SOLARIUS24::CODIGO::ESTADO_REDE: return "ESTADO_REDE";
                case SOLARIUS24::CODIGO::ESTADO_URGENCIA: return "ESTADO_URGENCIA";
                case SOLARIUS24::CODIGO::CMD_LIGAR_FONTE_USINA: return "CMD_LIGAR_FONTE_USINA";
                case SOLARIUS24::CODIGO::CMD_STATUS_GERAL: return "CMD_STATUS_GERAL";
                case SOLARIUS24::CODIGO::CMD_LORA_PING: return "CMD_LORA_PING";
                case SOLARIUS24::CODIGO::ACK_STATUS_GERAL_FIM: return "ACK_STATUS_GERAL_FIM";
                case SOLARIUS24::CODIGO::SISTEMA_BOOT: return "SISTEMA_BOOT";

                default: return "SOLARIUS24_DESCONHECIDO";
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
            switch((ECO_HIDRO::CODIGO)codigo)
            {
                case ECO_HIDRO::CODIGO::NIVEL_BOIAS: return "NIVEL_BOIAS";
                case ECO_HIDRO::CODIGO::STATUS_BOMBA_CASA: return "STATUS_BOMBA_CASA";
                case ECO_HIDRO::CODIGO::STATUS_BOMBA_RESERVATORIO: return "STATUS_BOMBA_RESERVATORIO";
                case ECO_HIDRO::CODIGO::VOLTAGEM_SISTEMA: return "VOLTAGEM_SISTEMA";
                case ECO_HIDRO::CODIGO::CMD_LIGAR_BOMBA_CASA: return "CMD_LIGAR_BOMBA_CASA";
                case ECO_HIDRO::CODIGO::CMD_DESLIGAR_BOMBA_CASA: return "CMD_DESLIGAR_BOMBA_CASA";
                case ECO_HIDRO::CODIGO::CMD_DESLIGAR_BOMBA_RESERVATORIO: return "CMD_DESLIGAR_BOMBA_RESERVATORIO";
                case ECO_HIDRO::CODIGO::CMD_STATUS_GERAL: return "CMD_STATUS_GERAL";
                case ECO_HIDRO::CODIGO::CMD_LORA_PING: return "CMD_LORA_PING";
                case ECO_HIDRO::CODIGO::CMD_BLOQUEADO: return "CMD_BLOQUEADO";
                case ECO_HIDRO::CODIGO::CMD_NORMAL: return "CMD_NORMAL";
                case ECO_HIDRO::CODIGO::CMD_PORTAO_ABRIR: return "CMD_PORTAO_ABRIR";
                case ECO_HIDRO::CODIGO::CMD_SENHA_PROTECAO_PORTAO: return "CMD_SENHA_PROTECAO_PORTAO";
                case ECO_HIDRO::CODIGO::CMD_CICLO_COMPLETO: return "CMD_CICLO_COMPLETO";
                case ECO_HIDRO::CODIGO::CMD_MEIO_CICLO: return "CMD_MEIO_CICLO";
                case ECO_HIDRO::CODIGO::ACK_STATUS_GERAL_FIM: return "ACK_STATUS_GERAL_FIM";
                case ECO_HIDRO::CODIGO::SISTEMA_BOOT: return "SISTEMA_BOOT";

                default: return "ECO_HIDRO_DESCONHECIDO";
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
            switch((GATE_DRIMEL::CODIGO)codigo)
            {
                case GATE_DRIMEL::CODIGO::STATUS_POSICAO: return "STATUS_POSICAO";
                case GATE_DRIMEL::CODIGO::SENSOR_PRESENCA: return "SENSOR_PRESENCA";
                case GATE_DRIMEL::CODIGO::TEMPERATURA: return "TEMPERATURA";
                case GATE_DRIMEL::CODIGO::UMIDADE: return "UMIDADE";
                case GATE_DRIMEL::CODIGO::CMD_ABRIR: return "CMD_ABRIR";
                case GATE_DRIMEL::CODIGO::CMD_FECHAR: return "CMD_FECHAR";
                case GATE_DRIMEL::CODIGO::CMD_SENHA: return "CMD_SENHA";
                case GATE_DRIMEL::CODIGO::CMD_STATUS_GERAL: return "CMD_STATUS_GERAL";
                case GATE_DRIMEL::CODIGO::CMD_LORA_PING: return "CMD_LORA_PING";
                case GATE_DRIMEL::CODIGO::CMD_BLOQUEADO: return "CMD_BLOQUEADO";
                case GATE_DRIMEL::CODIGO::CMD_NORMAL: return "CMD_NORMAL";
                case GATE_DRIMEL::CODIGO::CMD_FERIAS: return "CMD_FERIAS";
                case GATE_DRIMEL::CODIGO::CMD_FESTA: return "CMD_FESTA";
                case GATE_DRIMEL::CODIGO::ACK_STATUS_GERAL_FIM: return "ACK_STATUS_GERAL_FIM";
                case GATE_DRIMEL::CODIGO::SISTEMA_BOOT: return "SISTEMA_BOOT";

                default: return "GATE_DRIMEL_DESCONHECIDO";
            }

        default:    return "SISTEMA_INVALIDO";
          
    }
}

//====================================================================================
//                   DEBUG_PACOTE
//====================================================================================

inline void PrintPacote(const Pacote& p)
{
    uint8_t destino   = ObterDestino(p.sistema);
    uint8_t remetente = ObterRemetente(p.sistema);

    debugVariavel("[", SistemaParaTexto(remetente), NEGRITO, AMARELO, LARANJA);
    debugVariavel(" -> ", SistemaParaTexto(destino), NEGRITO, AMARELO, LARANJA);
    debugVariavel("] ", CodigoParaTexto(destino, p.codigo), NEGRITO, AMARELO, LARANJA);
    debugVariavel(" = ", p.getValor(), NEGRITO, AMARELO, LARANJA);
}

}