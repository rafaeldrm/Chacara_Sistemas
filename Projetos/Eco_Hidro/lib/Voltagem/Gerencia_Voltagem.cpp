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
 
 
} 
 
//====================================================================================  
//                   LOOP  
//====================================================================================  
 
 void Gerencia_Voltagem_Atualiza() 
{ 

switch (VOLTAGEM::atual()) {

//============================================================================
  
    case VOLTAGEM::LEITURA::NORMAL:

    Controle_Voltagem_Normal();

        break;

//============================================================================

    case VOLTAGEM::LEITURA::IDEAL:

    Controle_Voltagem_Ideal();

        break;

//============================================================================
    case VOLTAGEM::LEITURA::CHEIA:

    Controle_Voltagem_Cheia();
        
        break;

    //============================================================================
    case VOLTAGEM::LEITURA::MANTENDO_CARGA:

    Controle_Voltagem_Mantendo_Carga();
        
        break;

    //============================================================================
    case VOLTAGEM::LEITURA::CARGA_CONSUMIDA:

    Controle_Voltagem_Carga_Consumida();
        
        break;

//============================================================================

    case VOLTAGEM::LEITURA::CRITICO:

    Controle_Voltagem_Critico();

        break;

//============================================================================

    default:

        VOLTAGEM::definir(VOLTAGEM::LEITURA::NORMAL);

        break;
    
    }
}