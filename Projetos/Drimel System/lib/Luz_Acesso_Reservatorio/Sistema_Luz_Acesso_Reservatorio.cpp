//====================================================================================  
//                   DEPENDÊNCIAS  
//====================================================================================  
 
#include "Sistema_Luz_Acesso_Reservatorio.h" 
#include "DEBUG_MODE.h" 

#include "Controle_Luz_Acesso_Reservatorio.h"
#include "Gerencia_Luz_Acesso_Reservatorio.h"
 
//====================================================================================  
//                   VARIÁVEIS INTERNAS  
//====================================================================================  

//====================================================================================  
//                   SETUP  
//====================================================================================  
 
 void LUZ_ACESSO_RESERVATORIO_BOOT() 
{ 
    debugSetupArquivo();
    debugSetupDivisao(COR::AMARELO,  "SISTEMA LUZ ACESSO RESERVATORIO INICIALIZANDO", COR::MAGENTA, "=", 45);
    
    Controle_Luz_Acesso_Reservatorio_Inicializa();

    Gerencia_Luz_Acesso_Reservatorio_Inicializa();
} 
 
//====================================================================================  
//                   LOOP  
//====================================================================================  
 
 void LUZ_ACESSO_RESERVATORIO_LOOP() 
{ 

    Gerencia_Luz_Acesso_Reservatorio_Atualiza();
 
} 
 
