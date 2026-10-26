//====================================================================================  
//                   DEPENDÊNCIAS  
//====================================================================================  
 
#include "Sistema_Casa_Torre.h" 
#include "DEBUG_MODE.h" 

#include "Gerencia_Casa_Torre.h"
#include "Controle_Casa_Torre.h"
#include "Controle_Bomba_Casa.h"
#include "Gerencia_Bomba_Casa.h"
#include "Notificacao_Casa_Torre.h"

//====================================================================================  
//                   VARIÁVEIS INTERNAS  
//====================================================================================  


 
//====================================================================================  
//                   SETUP  
//====================================================================================  
 
 void CASA_TORRE_BOOT()
{ 
    debugSetupArquivo();
    debugSetupDivisao(COR::AMARELO, "SISTEMA CASA TORRE INICIALIZANDO", COR::MAGENTA, "=", 45);

    Gerencia_Casa_Torre_Inicializa();
 
    Controle_Casa_Torre_Inicializa();

    Gerencia_Bomba_Casa_Inicializa();

    Controle_Bomba_Casa_Inicializa();

    Notificacao_Casa_Torre_Inicializa();
} 
 
//====================================================================================  
//                   LOOP  
//====================================================================================  
 
 void CASA_TORRE_LOOP()
{

    Gerencia_Casa_Torre_Atualiza();

    Gerencia_Bomba_Casa_Atualiza();

    Notificacao_Casa_Torre_Bomba_Acao();
 
} 