//====================================================================================  
//                   DEPENDÊNCIAS  
//====================================================================================  
 
#include "Controle_Bomba_12V.h" 
#include "DEBUG_MODE.h"

#include "Estados_Sistema_Eco_Hidro.h"
#include "Modulo_Reles.h"
 
//====================================================================================  
//                   VARIÁVEIS INTERNAS  
//====================================================================================  
 


//====================================================================================  
//                   SETUP  
//====================================================================================  
 
 void Controle_Bomba_12V_Inicializa() 
{ 
        debugSetupDivisao(COR::AMARELO, "CONTROLE BOMBA 12V INICIALIZANDO", COR::MAGENTA, "=", 45);
} 
 
//====================================================================================  
//                   LIBERADO
//====================================================================================  
 
 void Controle_Bomba_12V_Liberado() 
{

    BOMBA_12V::definir(BOMBA_12V::ESTAGIOS::AGUARDA_LIBERACAO);
           
}

//====================================================================================
//                   LIGANDO BOMBA RESERVATÓRIO
//====================================================================================


void Controle_Bomba_12V_Ligando_Bomba()
{

        RELE_BOMBA_RESERVA_12V.Ligar(); //@@ Modulo_Reles.cpp

        BOMBA_12V::definir(BOMBA_12V::ESTAGIOS::BOMBA_LIGADA);

}

//====================================================================================
//                   DESLIGANDO BOMBA RESERVATÓRIO
//====================================================================================

void Controle_Bomba_12V_Desligando_Bomba()
{

        RELE_BOMBA_RESERVA_12V.Desligar();
        BOMBA_12V::definir(BOMBA_12V::ESTAGIOS::REPOUSO);

}