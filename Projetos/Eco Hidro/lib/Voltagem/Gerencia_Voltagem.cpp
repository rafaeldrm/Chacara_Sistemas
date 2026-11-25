//====================================================================================  
//                   DEPENDÊNCIAS  
//====================================================================================  
 
#include "Gerencia_Voltagem.h" 
#include "DEBUG_MODE.h" 

#include "Controle_Voltagem.h"
#include "Estados_Sistema_Eco_Hidro.h"

//====================================================================================  
//                   VARIÁVEIS INTERNAS  
//==================================================================================== 
 
//====================================================================================  
//                   SETUP  
//====================================================================================  
 
 void Gerencia_Voltagem_Inicializa() 
{ 
        debugSetupDivisao(COR::AMARELO, "GERÊNCIA VOLTAGEM INICIALIZANDO", COR::MAGENTA, "=", 0);
        VOLTAGEM::definir(VOLTAGEM::ESTADO::NORMAL);
} 
 
//====================================================================================  
//                   LOOP  
//====================================================================================  
 
 void Gerencia_Voltagem_Atualiza() 
{ 

    switch (VOLTAGEM::atual())
    {

//============================================================================
  
        case VOLTAGEM::ESTADO::NORMAL:

            Controle_Voltagem_Normal();

        break;

//============================================================================

        case VOLTAGEM::ESTADO::IDEAL:

            Controle_Voltagem_Ideal();

        break;

//============================================================================
        case VOLTAGEM::ESTADO::CHEIA:

            Controle_Voltagem_Cheia();
        
        break;

//============================================================================
        case VOLTAGEM::ESTADO::MANTENDO_CARGA:

            Controle_Voltagem_Mantendo_Carga();
        
        break;

//============================================================================
        case VOLTAGEM::ESTADO::CARGA_CONSUMIDA:

            Controle_Voltagem_Carga_Consumida();
        
        break;

//============================================================================

        case VOLTAGEM::ESTADO::CRITICO:

            Controle_Voltagem_Critico();

        break;

//============================================================================

        case VOLTAGEM::ESTADO::CARGA_PRIORIDADE:

        // não tem função
        
        break;

//============================================================================

        default:

            VOLTAGEM::definir(VOLTAGEM::ESTADO::NORMAL);

        break;
    }
}