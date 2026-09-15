//====================================================================================  
//                   DEPENDÊNCIAS  
//====================================================================================  
 
#include "Gerencia_Bomba_Casa.h" 
#include "DEBUG_MODE.h" 

#include "Controle_Bomba_Casa.h"
#include "Estados_Sistema_Eco_Hidro.h"
 
//====================================================================================  
//                   VARIÁVEIS INTERNAS  
//====================================================================================  
 

 
//====================================================================================  
//                   SETUP  
//====================================================================================  
 
 void Gerencia_Bomba_Casa_Inicializa() 
{ 

 
} 
 
//====================================================================================  
//                   LOOP  
//====================================================================================  
 
 void Gerencia_Bomba_Casa_Atualiza() 
{ 

    switch (BOMBA_CASA::atual()) {

    //========================================================
    case BOMBA_CASA::ESTAGIOS::REPOUSO:
        // não faz nada
    break;

    //========================================================
    case BOMBA_CASA::ESTAGIOS::LIBERADO:

        Controle_Bomba_Casa_Liberado();     

    break;

    //========================================================
    case BOMBA_CASA::ESTAGIOS::AGUARDA_LIBERACAO:

      //Apenas o Mediador fará a alteração.

    break;

    //========================================================
    case BOMBA_CASA::ESTAGIOS::LIGANDO_BOMBA:

      Controle_Bomba_Casa_Ligando_Bomba();

    break;

    //========================================================
    case BOMBA_CASA::ESTAGIOS::BOMBA_LIGADA:
        //apenas fica aqui quieto, funcionando até alguém tirar ele daqui.
    break;

    //========================================================
    case BOMBA_CASA::ESTAGIOS::DESLIGANDO_BOMBA:

      Controle_Bomba_Casa_Desligando_Bomba();

    break;
    
    }
}