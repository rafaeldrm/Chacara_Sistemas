//====================================================================================
//                   DEPENDÊNCIAS
//====================================================================================

#include "Gerencia_Tranco_Bomba_Casa.h"
#include "DEBUG_MODE.h"

#include "FSM/Estados_Sistema_Eco_Hidro.h"
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
        TRANCO_BOMBA_CASA::definir(TRANCO_BOMBA_CASA::ESTAGIOS::REPOUSO);
}

//====================================================================================
//                   LOOP
//====================================================================================

void Gerencia_Tranco_Bomba_Casa_Atualiza()
{
    switch (TRANCO_BOMBA_CASA::atual())
    {
        case TRANCO_BOMBA_CASA::ESTAGIOS::REPOUSO:
        
        break;

//==============================================

        case TRANCO_BOMBA_CASA::ESTAGIOS::INICIANDO_MONITORAMENTO_HELICE:

            Controle_Iniciando_Monitoramento_Helice();

        break;

//==============================================

        case TRANCO_BOMBA_CASA::ESTAGIOS::BOMBA_TRAVADA:

            Controle_Bomba_Travada();
        
        break;

//==============================================

        case TRANCO_BOMBA_CASA::ESTAGIOS::PRIMEIRA_TENTATIVA_TRANCO:

            Controle_Primeira_Tentativa_Tranco();
        
        break;

//==============================================

        case TRANCO_BOMBA_CASA::ESTAGIOS::SEGUNDA_TENTATIVA_TRANCO:

            Controle_Segunda_Tentativa_Tranco();

        break;

//==============================================

        case TRANCO_BOMBA_CASA::ESTAGIOS::SENSOR_IR_DESATIVADO:

            Controle_Bomba_Sensor_Ir_Desativado();
        
        break;

//==============================================

        case TRANCO_BOMBA_CASA::ESTAGIOS::BOMBA_LIBERADA:    

            Controle_Bomba_Liberada();
   
        break;

//==============================================

        default:

        
        break;

        }
}
