//====================================================================================  
//                   DEPENDÊNCIAS  
//====================================================================================  
 
#include "Controle_Bomba_Casa.h" 
#include "DEBUG_MODE.h"

#include "Estados_Sistema_Eco_Hidro.h"
#include "Modulo_Reles.h"
 
//====================================================================================  
//                   VARIÁVEIS INTERNAS  
//====================================================================================  



//====================================================================================  
//                   SETUP  
//====================================================================================  
 
 void Controle_Bomba_Casa_Inicializa() 
{ 
        debugSetupDivisao(COR::AMARELO, "CONTROLE BOMBA CASA INICIALIZANDO", COR::MAGENTA, "=", 45);       
} 
 
//====================================================================================  
//                   LIBERADO
//====================================================================================  
 
 void Controle_Bomba_Casa_Liberado() 
{ 
            BOMBA_CASA::definir(BOMBA_CASA::ESTAGIOS::AGUARDA_LIBERACAO);
 
} 

//====================================================================================
//                   LIGANDO BOMBA
//====================================================================================

void Controle_Bomba_Casa_Ligando_Bomba()
{

        RELE_BOMBA_CASA.Ligar(); //@@ Modulo_Reles.cpp

        BOMBA_CASA::definir(BOMBA_CASA::ESTAGIOS::BOMBA_LIGADA);

}

//====================================================================================
//                   DESLIGANDO BOMBA
//====================================================================================

 void Controle_Bomba_Casa_Desligando_Bomba() 
{

        RELE_BOMBA_CASA.Desligar(); //@@ Modulo_Reles.cpp

        BOMBA_CASA::definir(BOMBA_CASA::ESTAGIOS::REPOUSO);

}