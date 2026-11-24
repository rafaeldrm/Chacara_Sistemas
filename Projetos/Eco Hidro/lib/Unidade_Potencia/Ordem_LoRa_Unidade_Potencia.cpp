//====================================================================================
//                   DEPENDÊNCIAS
//====================================================================================

#include "Ordem_LoRa_Unidade_Potencia.h"
#include "DEBUG_MODE.h"

#include "FSM/Estados_Sistema_Eco_Hidro.h"
#include "Eventos_FSM_LoRa/Estados_Eventos_Eco_Hidro.h"

//====================================================================================
//                   VARIÁVEIS INTERNAS
//====================================================================================

//====================================================================================
//                   SETUP
//====================================================================================

void Ordem_LoRa_Unidade_Potencia_Inicializa()
{
        debugSetupDivisao(COR::AMARELO, "ORDEM LORA UNIDADE POTENCIA INICIALIZANDO", COR::MAGENTA, "=", 0);
}

//====================================================================================
//                   ACIONAMENTO FORÇADO - POSSÍVEL TRAVAMENTO
//====================================================================================

static void Ordem_LoRa_Unidade_Potencia_Ordem_Acionamento()
{
    const auto ordemUnidadePotencia = ORDENS_DRIMEL_SYSTEM::UNIDADE_POTENCIA::atual();

    if (ordemUnidadePotencia == ORDENS_DRIMEL_SYSTEM::UNIDADE_POTENCIA::ORDEM::ACIONA)
    {
        UNIDADE_POTENCIA::definir(UNIDADE_POTENCIA::ESTAGIOS::ACIONAMENTO_RELES);
        ORDENS_DRIMEL_SYSTEM::UNIDADE_POTENCIA::definir(ORDENS_DRIMEL_SYSTEM::UNIDADE_POTENCIA::ORDEM::NENHUM);
    }
}

//====================================================================================
//                   DESLIGAMENTO FORÇADO - POSSÍVEL TRAVAMENTO
//====================================================================================

static void Ordem_LoRa_Unidade_Potencia_Ordem_Desligamento()
{
    const auto ordemUnidadePotencia = ORDENS_DRIMEL_SYSTEM::UNIDADE_POTENCIA::atual();

    if (ordemUnidadePotencia == ORDENS_DRIMEL_SYSTEM::UNIDADE_POTENCIA::ORDEM::DESLIGA)
    {
        UNIDADE_POTENCIA::definir(UNIDADE_POTENCIA::ESTAGIOS::DESLIGANDO);
        ORDENS_DRIMEL_SYSTEM::UNIDADE_POTENCIA::definir(ORDENS_DRIMEL_SYSTEM::UNIDADE_POTENCIA::ORDEM::NENHUM);
    }
}

//====================================================================================
//                   LOOP
//====================================================================================

void Ordem_LoRa_Unidade_Potencia_Atualiza()
{
    Ordem_LoRa_Unidade_Potencia_Ordem_Acionamento();

    Ordem_LoRa_Unidade_Potencia_Ordem_Desligamento();
}
