//====================================================================================
//                   DEPENDÊNCIAS
//====================================================================================

#include "Mediador_Tranco_Bomba_Casa.h"
#include "DEBUG_MODE.h"

#include "Estados_Sistema_Eco_Hidro.h"

//====================================================================================
//                   VARIÁVEIS INTERNAS
//====================================================================================

//====================================================================================
//                   SETUP
//====================================================================================

void Mediador_Tranco_Bomba_Casa_Inicializa()
{
        debugSetupDivisao(COR::AMARELO, "MEDIADOR TRANCO BOMBA CASA INICIALIZANDO", COR::MAGENTA, "=", 0);
}



//====================================================================================
//                   LOOP
//====================================================================================

void Mediador_Tranco_Bomba_Casa_Atualiza()
{

       const auto bombaCasa = BOMBA_CASA::atual();
       const auto trancoBomba = TRANCO_BOMBA_CASA::atual();

    if (trancoBomba == TRANCO_BOMBA_CASA::ESTADO::REPOUSO
        && (bombaCasa == BOMBA_CASA::ESTADO::LIGANDO_BOMBA 
    || bombaCasa == BOMBA_CASA::ESTADO::BOMBA_LIGADA))
    {
        TRANCO_BOMBA_CASA::definir(TRANCO_BOMBA_CASA::ESTADO::INICIANDO_MONITORAMENTO_HELICE);
    }
}
