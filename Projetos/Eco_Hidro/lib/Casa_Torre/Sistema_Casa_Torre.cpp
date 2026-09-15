//====================================================================================  
//                   DEPENDÊNCIAS  
//====================================================================================  
 
#include "Sistema_Casa_Torre.h" 
#include "DEBUG_MODE.h" 

#include "Gerencia_Casa_Torre.h"
#include "Controle_Casa_Torre.h"
#include "Controle_Bomba_Casa.h"
#include "Gerencia_Bomba_Casa.h"
#include "Ordens_Drimel_System_Casa_Torre.h"

#include "Estados_Sistema_Eco_Hidro.h"

//====================================================================================  
//                   VARIÁVEIS INTERNAS  
//====================================================================================  


 
//====================================================================================  
//                   SETUP  
//====================================================================================  
 
 void CASA_TORRE_BOOT()
{ 
 
    Gerencia_Casa_Torre_Inicializa();
 
    Controle_Casa_Torre_Inicializa();

    Gerencia_Bomba_Casa_Inicializa();

    Controle_Bomba_Casa_Inicializa();

} 
 
//====================================================================================  
//                   LOOP  
//====================================================================================  
 
 void CASA_TORRE_LOOP()
{ 
 
    if (SISTEMA_ECO_HIDRO::atual() == SISTEMA_ECO_HIDRO::MODO::BLOQUEIO) 
    {
        return; //se for bloqueio do sistema logo nunca acessará essa parte do if.)
    }

    Gerencia_Casa_Torre_Atualiza();

    Gerencia_Bomba_Casa_Atualiza();
 
} 