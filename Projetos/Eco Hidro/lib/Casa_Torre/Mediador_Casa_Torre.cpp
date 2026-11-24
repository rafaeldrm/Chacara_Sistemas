//====================================================================================
//                   DEPENDÊNCIAS
//====================================================================================

#include "Mediador_Casa_Torre.h"
#include "DEBUG_MODE.h"

#include "FSM/Estados_Sistema_Eco_Hidro.h"

//====================================================================================
//                   VARIÁVEIS INTERNAS
//====================================================================================

//====================================================================================
//                   SETUP
//====================================================================================

void Mediador_Casa_Torre_Inicializa()
{
        debugSetupDivisao(COR::AMARELO, "MEDIADOR CASA TORRE INICIALIZANDO", COR::MAGENTA, "=", 0);
}

//@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@

//                      CASA TORRE - PROTEÇÃO TÉRMICA

//@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@

//====================================================================================
//                   RETORNA DE ONDE PAROU
//====================================================================================

static void Mediador_Casa_Torre_Protecao_Termica_Retorna_Onde_Parou()
{
    const auto termicoBomba = PROTECAO_TERMICA_BOMBA_CASA::atual();
    const auto bombaCasa = BOMBA_CASA::atual();

    if (termicoBomba == PROTECAO_TERMICA_BOMBA_CASA::SITUACAO::MODO_SEGURANCA_PRIMEIRO_CICLO
        && bombaCasa == BOMBA_CASA::ESTAGIOS::REPOUSO)
    {
        CASA_TORRE::definir(CASA_TORRE::ETAPAS::PRIMEIRO_CICLO_ENCHIMENTO_CAIXA);
    }

    //==========================

     if (termicoBomba == PROTECAO_TERMICA_BOMBA_CASA::SITUACAO::MODO_SEGURANCA_SEGUNDO_CICLO
        && bombaCasa == BOMBA_CASA::ESTAGIOS::REPOUSO)
    {
        CASA_TORRE::definir(CASA_TORRE::ETAPAS::SEGUNDO_CICLO_ENCHIMENTO_CAIXA);
    }
}

//====================================================================================
//                   DESLIGAMENTO TEMPERATURA BOMBA
//====================================================================================

static void Mediador_Casa_Torre_Protecao_Termica_Bomba_Desliga()
{
      const auto termicoBomba = PROTECAO_TERMICA_BOMBA_CASA::atual();
      const auto bombaCasa = BOMBA_CASA::atual();

      if (bombaCasa == BOMBA_CASA::ESTAGIOS::BOMBA_LIGADA
        && termicoBomba == PROTECAO_TERMICA_BOMBA_CASA::SITUACAO::SUPERAQUECIMENTO)
        {
            CASA_TORRE::definir(CASA_TORRE::ETAPAS::BLOQUEIO);
        }
}

//@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@

//                      CASA TORRE - RESERVATÓRIO

//@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@

//====================================================================================
//                   RESETANDO POSICIONAMENTO DA CASA TORRE
//====================================================================================

static void Mediador_Casa_Torre_Finalizado_Reservatorio()
{
    const auto reservatorio = RESERVATORIO::atual();
    const auto casaTorre = CASA_TORRE::atual();
    
    if (casaTorre == CASA_TORRE::ETAPAS::LIBERADO_PARA_O_RESERVATORIO
        && (reservatorio == RESERVATORIO::TRANSFERENCIA::BLOQUEIO_SEGURANCA
        || reservatorio == RESERVATORIO::TRANSFERENCIA::FINALIZADO))
    {
        CASA_TORRE::definir(CASA_TORRE::ETAPAS::NORMAL);
    }
}

//@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@

//====================================================================================
//                   LOOP
//====================================================================================

void Mediador_Casa_Torre_Atualiza()
{
        Mediador_Casa_Torre_Finalizado_Reservatorio();

        Mediador_Casa_Torre_Protecao_Termica_Retorna_Onde_Parou();
        
        Mediador_Casa_Torre_Protecao_Termica_Bomba_Desliga();
}