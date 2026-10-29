//====================================================================================  
//                   DEPENDÊNCIAS  
//====================================================================================  
 
#include "Gerencia_Bomba_12V.h" 
#include "DEBUG_MODE.h"

#include "Estados_Sistema_Eco_Hidro.h"
#include "Controle_Bomba_12V.h"
 
//====================================================================================  
//                   VARIÁVEIS INTERNAS  
//====================================================================================  
 


//====================================================================================  
//                   SETUP  
//====================================================================================  
 
 void Gerencia_Bomba_12V_Inicializa() 
{ 
        debugSetupDivisao(COR::AMARELO, "GERÊNCIA BOMBA 12V INICIALIZANDO", COR::MAGENTA, "=", 45);
} 
 
//====================================================================================  
//                   LOOP  
//====================================================================================  
 
 void Gerencia_Bomba_12V_Atualiza() 
{ 

    switch (BOMBA_12V::atual()) {

    //========================================================
    case BOMBA_12V::ESTAGIOS::REPOUSO:

    break;

    //========================================================
    case BOMBA_12V::ESTAGIOS::LIBERADO:

    Controle_Bomba_12V_Liberado();

    break;

    //========================================================
    case BOMBA_12V::ESTAGIOS::AGUARDA_LIBERACAO:

   //Apenas o Mediador fará alteração.

    break;
        
    //========================================================
    case BOMBA_12V::ESTAGIOS::LIGANDO_BOMBA:

    Controle_Bomba_12V_Ligando_Bomba();

    break;

    //========================================================
    case BOMBA_12V::ESTAGIOS::BOMBA_LIGADA:
        //apenas fica aqui quieto, funcionando até alguém tirar ele daqui.
    break;

    //========================================================
    case BOMBA_12V::ESTAGIOS::DESLIGANDO_BOMBA:
      
      Controle_Bomba_12V_Desligando_Bomba();

    break;

    }
}