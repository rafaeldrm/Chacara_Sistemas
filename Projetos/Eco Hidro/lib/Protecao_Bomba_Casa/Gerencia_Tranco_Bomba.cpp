//====================================================================================
//                   DEPENDÊNCIAS
//====================================================================================

#include "Gerencia_Tranco_Bomba.h"
#include "DEBUG_MODE.h"

#include "Estados_Sistema_Eco_Hidro.h"
#include "Controle_Tranco_Bomba.h"

//====================================================================================
//                   VARIÁVEIS INTERNAS
//====================================================================================

//====================================================================================
//                   SETUP
//====================================================================================

void Gerencia_Tranco_Bomba_Inicializa()
{
        debugSetupDivisao(COR::AMARELO, "GERENCIA TRANCO BOMBA INICIALIZANDO", COR::MAGENTA, "=", 45);
}

//====================================================================================
//                   LOOP
//====================================================================================

void Gerencia_Tranco_Bomba_Atualiza()
{
    switch (TRANCO_BOMBA::atual())
    {
        case TRANCO_BOMBA::ESTAGIOS::REPOUSO:
        
        break;

//==============================================

        case TRANCO_BOMBA::ESTAGIOS::INICIANDO_MONITORAMENTO_HELICE:

            Controle_Iniciando_Monitoramento_Helice();

        break;

//==============================================

        case TRANCO_BOMBA::ESTAGIOS::BOMBA_TRAVADA:

            Controle_Bomba_Travada();
        
        break;

//==============================================

        case TRANCO_BOMBA::ESTAGIOS::PRIMEIRA_TENTATIVA_TRANCO:

            Controle_Primeira_Tentativa_Tranco();
        
        break;

//==============================================

        case TRANCO_BOMBA::ESTAGIOS::SEGUNDA_TENTATIVA_TRANCO:

            Controle_Segunda_Tentativa_Tranco();

        break;

//==============================================

        case TRANCO_BOMBA::ESTAGIOS::SENSOR_IR_DESATIVADO:

            Controle_Bomba_Sensor_Ir_Desativado();
        
        break;

//==============================================

        case TRANCO_BOMBA::ESTAGIOS::BOMBA_LIBERADA:    

            Controle_Bomba_Liberada();
   
        break;

//==============================================

        default:

        
        break;

        }
}
