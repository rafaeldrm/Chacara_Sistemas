//====================================================================================
//                   DEPENDÊNCIAS
//====================================================================================

#include "Mediador_Termico_Bomba_Casa.h"
#include "DEBUG_MODE.h"

#include "Estados_Sistema_Eco_Hidro.h"

//====================================================================================
//                   VARIÁVEIS INTERNAS
//====================================================================================

//====================================================================================
//                   SETUP
//====================================================================================

void Mediador_Termico_Bomba_Casa_Inicializa()
{
        debugSetupDivisao(COR::AMARELO, "MEDIADOR TERMICO BOMBA CASA INICIALIZANDO", COR::MAGENTA, "=", 0);
}

//====================================================================================
//                   ATIVAMENTO
//====================================================================================

static void Mediador_Termico_Bomba_Casa_Ativa_Monitoramento()
{
    const auto bombaCasa = BOMBA_CASA::atual();
    const auto termicoBomba = PROTECAO_TERMICA_BOMBA_CASA::atual();

    if (bombaCasa == BOMBA_CASA::ESTADO::BOMBA_LIGADA
        && termicoBomba == PROTECAO_TERMICA_BOMBA_CASA::ESTADO::REPOUSO)
    {
        PROTECAO_TERMICA_BOMBA_CASA::definir(PROTECAO_TERMICA_BOMBA_CASA::ESTADO::MONITORAMENTO_ATIVADO);
    }
}

//====================================================================================
//                   PROTECAO TERMICA DESLIGANDO
//====================================================================================

static void Mediador_Protecao_Termica_Bomba_Casa_Desligando()
{
    const auto casaTorre = CASA_TORRE::atual();
    const auto termicoBomba = PROTECAO_TERMICA_BOMBA_CASA::atual();

    if (termicoBomba != PROTECAO_TERMICA_BOMBA_CASA::ESTADO::REPOUSO
        && (casaTorre == CASA_TORRE::ESTADO::AGUARDANDO_TRANSFERENCIA_GRAVIDADE
        || casaTorre == CASA_TORRE::ESTADO::LIBERADO_PARA_O_RESERVATORIO))
    {
        PROTECAO_TERMICA_BOMBA_CASA::definir(PROTECAO_TERMICA_BOMBA_CASA::ESTADO::FINALIZADO);
    }

}

//====================================================================================
//                   LOOP
//=============================================================s=======================

void Mediador_Termico_Bomba_Casa_Atualiza()
{
    Mediador_Termico_Bomba_Casa_Ativa_Monitoramento();

     Mediador_Protecao_Termica_Bomba_Casa_Desligando();

}
