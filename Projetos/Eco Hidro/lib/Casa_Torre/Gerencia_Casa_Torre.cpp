//====================================================================================  
//                   DEPENDÊNCIAS  
//====================================================================================  
 
#include "Gerencia_Casa_Torre.h" 
#include "DEBUG_MODE.h" 

#include "Controle_Casa_Torre.h"
#include "Estados_Sistema_Eco_Hidro.h"
 
//====================================================================================  
//                   VARIÁVEIS INTERNAS  
//====================================================================================  
 
 
//====================================================================================  
//                   SETUP  
//====================================================================================  
 
void Gerencia_Casa_Torre_Inicializa()
{
        debugSetupDivisao(COR::AMARELO, "GERÊNCIA CASA TORRE INICIALIZANDO", COR::MAGENTA, "=", 45);
}
 
//====================================================================================  
//                   LOOP  
//====================================================================================  
 
 void Gerencia_Casa_Torre_Atualiza() 
{

    switch (CASA_TORRE::atual()) {
        
        //========================================================
        case CASA_TORRE::ETAPAS::NORMAL:
        
        case CASA_TORRE::ETAPAS::FINALIZADO:
            
        Controle_Casa_Torre_Normal();
          
        break;
        //========================================================
        case CASA_TORRE::ETAPAS::PRIMEIRO_CICLO_ENCHIMENTO_CAIXA:
        
            Controle_Casa_Torre_Primeiro_Ciclo_Enchimento_Caixa();
        
        break;
        
        //========================================================
        
        case CASA_TORRE::ETAPAS::AGUARDANDO_TRANSFERENCIA_GRAVIDADE:
        
            Controle_Casa_Torre_Aguardando_Transferencia_Gravidade();
        
        break;
        
        //========================================================
        
        case CASA_TORRE::ETAPAS::SEGUNDO_CICLO_ENCHIMENTO_CAIXA:
        
            Controle_Casa_Torre_Segundo_Ciclo_Enchimento_Caixa();
        
        break;
    }
}