//====================================================================================  
//                   DEPENDÊNCIAS  
//====================================================================================  
 
#include "Gerencia_Reservatorio.h" 
#include "DEBUG_MODE.h"
 
#include "Estados_Sistema_Eco_Hidro.h"
#include "Controle_Reservatorio.h"
 
//====================================================================================  
//                   VARIÁVEIS INTERNAS  
//====================================================================================  



//====================================================================================  
//                   SETUP  
//====================================================================================  
 
 void Gerencia_Reservatorio_Inicializa() 
{ 
        debugSetupDivisao(COR::AMARELO, "GERÊNCIA RESERVATÓRIO INICIALIZANDO", COR::MAGENTA, "=", 0);
        RESERVATORIO::definir(RESERVATORIO::ESTADO::NORMAL); 
} 
 
//====================================================================================  
//                   LOOP  
//====================================================================================  
 
 void Gerencia_Reservatorio_Atualiza() 
{ 

    switch (RESERVATORIO::atual()) {

        //========================================================
        case RESERVATORIO::ESTADO::NORMAL:
        break;

        //========================================================
        case RESERVATORIO::ESTADO::LIBERADO:
            Controle_Reservatorio_Liberado();
        break;

        //========================================================
        case RESERVATORIO::ESTADO::TRANSFERENCIA_RESERVATORIOS:
            Controle_Reservatorio_Transferencia_Reservatorios();
        break;

        //========================================================
        case RESERVATORIO::ESTADO::SO_MAIS_UM_POUQUINHO:
            Controle_Reservatorio_So_Mais_Um_Pouquinho();
        break;

        //========================================================
        case RESERVATORIO::ESTADO::FINALIZADO:
       //Não faz nada, fica travado até a Casa Torre o retirar desse estado (Mediador_Reservatorio)
        break;

        //========================================================
        case RESERVATORIO::ESTADO::BLOQUEIO_SEGURANCA:
        break;

        //========================================================
        default:
            RESERVATORIO::definir(RESERVATORIO::ESTADO::NORMAL);
        break;
    
    }
}