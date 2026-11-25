//====================================================================================  
//                   DEPENDÊNCIAS  
//====================================================================================  
 
#include "Gerencia_Casa_Torre.h" 
#include "DEBUG_MODE.h" 

#include "Controle_Casa_Torre.h"
#include "Estados_Sistema_Eco_Hidro.h"

#include "Rastreador_Trocas.h"
 
//====================================================================================  
//                   VARIÁVEIS INTERNAS  
//====================================================================================  
 
 
//====================================================================================  
//                   SETUP  
//====================================================================================  
 
void Gerencia_Casa_Torre_Inicializa()
{
        debugSetupDivisao(COR::AMARELO, "GERÊNCIA CASA TORRE INICIALIZANDO", COR::MAGENTA, "=", 0);
        CASA_TORRE::definir(CASA_TORRE::ESTADO::NORMAL);
}
 
//====================================================================================  
//                   LOOP  
//====================================================================================  
 
 void Gerencia_Casa_Torre_Atualiza() 
{

    //ORIGEM_FSM::setar(F("Gerencia_Casa_Torre"));

    switch (CASA_TORRE::atual()) {
        
        //========================================================
        case CASA_TORRE::ESTADO::NORMAL:
        
        case CASA_TORRE::ESTADO::LIBERADO_PARA_O_RESERVATORIO:
            
        Controle_Casa_Torre_Normal();
          
        break;
        //========================================================
        case CASA_TORRE::ESTADO::PRIMEIRO_CICLO_ENCHIMENTO_CAIXA:
        
            Controle_Casa_Torre_Primeiro_Ciclo_Enchimento_Caixa();
        
        break;
        
        //========================================================
        
        case CASA_TORRE::ESTADO::AGUARDANDO_TRANSFERENCIA_GRAVIDADE:
        
            Controle_Casa_Torre_Aguardando_Transferencia_Gravidade();
        
        break;
        
        //========================================================
        
        case CASA_TORRE::ESTADO::SEGUNDO_CICLO_ENCHIMENTO_CAIXA:
        
            Controle_Casa_Torre_Segundo_Ciclo_Enchimento_Caixa();
        
        break;

        //========================================================

        case CASA_TORRE::ESTADO::FINALIZADO:
        
            Controle_Casa_Torre_Finalizado();
        
            break;

        //=======================================================

        case CASA_TORRE::ESTADO::BLOQUEIO:

            //ainda sem motivos de fazer algo
            
        break;
            
    }
}