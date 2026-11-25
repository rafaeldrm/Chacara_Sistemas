//====================================================================================
//                   DEPENDÊNCIAS
//====================================================================================

#include "Ordem_LoRa_Casa_Torre.h"
#include "DEBUG_MODE.h"

#include "Estados_Eventos_Eco_Hidro.h"
#include "Estados_Sistema_Eco_Hidro.h"

//====================================================================================
//                   VARIÁVEIS INTERNAS
//====================================================================================

//====================================================================================
//                   SETUP
//====================================================================================

void Ordem_LoRa_Casa_Torre_Inicializa()
{
        debugSetupDivisao(COR::AMARELO, "ORDEM LORA CASA TORRE INICIALIZANDO", COR::MAGENTA, "=", 0);
}

//====================================================================================
//                   CICLO COMPLETO
//====================================================================================

static void Ordem_LoRa_Casa_Torre_Ciclo_Completo()
{

    const auto ordemCasaTorre = ORDENS_DRIMEL_SYSTEM::CASA_TORRE::atual();

    if (ordemCasaTorre == ORDENS_DRIMEL_SYSTEM::CASA_TORRE::ORDEM::CICLO_COMPLETO)
    {
        CASA_TORRE::definir(CASA_TORRE::ESTADO::PRIMEIRO_CICLO_ENCHIMENTO_CAIXA);
        ORDENS_DRIMEL_SYSTEM::CASA_TORRE::definir(ORDENS_DRIMEL_SYSTEM::CASA_TORRE::ORDEM::NENHUM);
    }
}

//====================================================================================
//                   MEIO CICLO
//====================================================================================

static void Ordem_LoRa_Casa_Torre_Meio_Ciclo()
{
    const auto ordemCasaTorre = ORDENS_DRIMEL_SYSTEM::CASA_TORRE::atual();

    if (ordemCasaTorre == ORDENS_DRIMEL_SYSTEM::CASA_TORRE::ORDEM::MEIO_CICLO)
    {
        CASA_TORRE::definir(CASA_TORRE::ESTADO::SEGUNDO_CICLO_ENCHIMENTO_CAIXA);
        ORDENS_DRIMEL_SYSTEM::CASA_TORRE::definir(ORDENS_DRIMEL_SYSTEM::CASA_TORRE::ORDEM::NENHUM);
    }
}

//====================================================================================
//                   NORMAL (Pós Bloqueio)
//====================================================================================

static void Ordem_LoRa_Casa_Torre_Normal()
{
    const auto ordemCasaTorre = ORDENS_DRIMEL_SYSTEM::CASA_TORRE::atual();

    if (ordemCasaTorre == ORDENS_DRIMEL_SYSTEM::CASA_TORRE::ORDEM::NORMAL)
    {
        CASA_TORRE::definir(CASA_TORRE::ESTADO::NORMAL);
        BOMBA_CASA::definir(BOMBA_CASA::ESTADO::DESLIGANDO_BOMBA); //automaticamente passa para REPOUSO
        ORDENS_DRIMEL_SYSTEM::CASA_TORRE::definir(ORDENS_DRIMEL_SYSTEM::CASA_TORRE::ORDEM::NENHUM);
    }
}

//====================================================================================
//                   BLOQUEIO (CASA_TORRE - BOMBA CASA)
//====================================================================================

static void Ordem_LoRa_Casa_Torre_Bloqueio()
{
    const auto ordemCasaTorre = ORDENS_DRIMEL_SYSTEM::CASA_TORRE::atual();

    if (ordemCasaTorre == ORDENS_DRIMEL_SYSTEM::CASA_TORRE::ORDEM::BLOQUEIA)
    {
        CASA_TORRE::definir(CASA_TORRE::ESTADO::BLOQUEIO);
        BOMBA_CASA::definir(BOMBA_CASA::ESTADO::BLOQUEIO); //Desliga o Relé
        ORDENS_DRIMEL_SYSTEM::CASA_TORRE::definir(ORDENS_DRIMEL_SYSTEM::CASA_TORRE::ORDEM::NENHUM);
    }

}

//====================================================================================
//                   LOOP
//====================================================================================

void Ordem_LoRa_Casa_Torre_Atualiza()
{
        Ordem_LoRa_Casa_Torre_Ciclo_Completo();

        Ordem_LoRa_Casa_Torre_Meio_Ciclo();

        Ordem_LoRa_Casa_Torre_Normal();

        Ordem_LoRa_Casa_Torre_Bloqueio();
}