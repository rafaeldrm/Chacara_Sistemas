//====================================================================================
//                   DEPENDÊNCIAS
//====================================================================================

#include "Gerencia_Tranco_Bomba_Casa.h"
#include "DEBUG_MODE.h"

#include "Estados_Sistema_Eco_Hidro.h"
#include "Controle_Tranco_Bomba_Casa.h"

//====================================================================================
//                   VARIÁVEIS INTERNAS
//====================================================================================

//====================================================================================
//                   SETUP
//====================================================================================

void Gerencia_Tranco_Bomba_Casa_Inicializa()
{
        debugSetupDivisao(COR::AMARELO, "GERENCIA TRANCO BOMBA CASA INICIALIZANDO", COR::MAGENTA, "=", 0);
        TRANCO_BOMBA_CASA::definir(TRANCO_BOMBA_CASA::ESTADO::REPOUSO);
}

//====================================================================================
//                   LOOP
//====================================================================================

void Gerencia_Tranco_Bomba_Casa_Atualiza()
{
    switch (TRANCO_BOMBA_CASA::atual())
    {
        case TRANCO_BOMBA_CASA::ESTADO::REPOUSO:
        
        break;

//==============================================

        case TRANCO_BOMBA_CASA::ESTADO::INICIANDO_MONITORAMENTO_HELICE:

            Controle_Tranco_Bomba_Casa_Iniciando_Monitoramento_Helice();

        break;

//==============================================

        case TRANCO_BOMBA_CASA::ESTADO::BOMBA_TRAVADA:

            Controle_Tranco_Bomba_Casa_Travada();
        
        break;

//==============================================

        case TRANCO_BOMBA_CASA::ESTADO::PRIMEIRA_TENTATIVA_TRANCO:

            Controle_Tranco_Bomba_Casa_Primeira_Tentativa_Tranco();
        
        break;

//==============================================

        case TRANCO_BOMBA_CASA::ESTADO::SEGUNDA_TENTATIVA_TRANCO:

            Controle_Tranco_Bomba_Casa_Segunda_Tentativa_Tranco();

        break;

//==============================================

        case TRANCO_BOMBA_CASA::ESTADO::SENSOR_IR_DESATIVADO:

            Controle_Tranco_Bomba_Casa_Sensor_Ir_Desativado();
        
        break;

//==============================================

        case TRANCO_BOMBA_CASA::ESTADO::BOMBA_LIBERADA:    

            Controle_Tranco_Bomba_Casa_Liberada();
   
        break;

//==============================================

        default:
        break;
        }
}
