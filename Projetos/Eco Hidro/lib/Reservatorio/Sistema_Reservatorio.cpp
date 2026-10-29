//====================================================================================  
//                   DEPENDÊNCIAS  
//====================================================================================  
 
#include "Sistema_Reservatorio.h" 
#include "DEBUG_MODE.h" 

#include "Gerencia_Bomba_12V.h"
#include "Gerencia_Reservatorio.h"
#include "Controle_Reservatorio.h"
#include "Controle_Bomba_12V.h"
#include "Notificacao_Reservatorio.h"

#include "Estados_Sistema_Eco_Hidro.h"

 
//====================================================================================  
//                   VARIÁVEIS INTERNAS  
//====================================================================================  
 
//====================================================================================  
//                   SETUP  
//====================================================================================  
 
 void RESERVATORIO_BOOT() 
{ 
    debugSetupArquivo();
    debugSetupDivisao(COR::AMARELO, "SISTEMA RESERVATÓRIO INICIALIZANDO", COR::MAGENTA, "=", 45);
    
    Gerencia_Reservatorio_Inicializa();

    Controle_Reservatorio_Inicializa();

    Gerencia_Bomba_12V_Inicializa();

    Controle_Bomba_12V_Inicializa();

    Notificacao_Reservatorio_Inicializa();
} 
 
//====================================================================================  
//                   LOOP  
//====================================================================================  
 
 void RESERVATORIO_LOOP() 
{ 
 
    Gerencia_Reservatorio_Atualiza();

    Gerencia_Bomba_12V_Atualiza();

    Notificacao_Reservatorio_Bomba_Acao();

} 