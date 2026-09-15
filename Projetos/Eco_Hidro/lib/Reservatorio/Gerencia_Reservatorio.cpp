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
 

 
} 
 
//====================================================================================  
//                   LOOP  
//====================================================================================  
 
 void Gerencia_Reservatorio_Atualiza() 
{ 

    switch (RESERVATORIO::atual()) {

        //========================================================

        case RESERVATORIO::TRANSFERENCIA::NORMAL:

        break;

        //========================================================

        case RESERVATORIO::TRANSFERENCIA::LIBERADO:

        Controle_Reservatorio_Liberado();

        break;

        //========================================================

        case RESERVATORIO::TRANSFERENCIA::SO_MAIS_UM_POUQUINHO:

        Controle_Reservatorio_So_Mais_Um_Pouquinho();

        break;

        //========================================================

        case RESERVATORIO::TRANSFERENCIA::FINALIZADO:

       //Não faz nada, fica travado até a Casa Torre o retirar desse estado (Mediador)

        break;

        //========================================================

        case RESERVATORIO::TRANSFERENCIA::BLOQUEIO_SEGURANCA:


        break;

        //========================================================

        default:

            RESERVATORIO::definir(RESERVATORIO::TRANSFERENCIA::NORMAL);

        break;
    
    }
}