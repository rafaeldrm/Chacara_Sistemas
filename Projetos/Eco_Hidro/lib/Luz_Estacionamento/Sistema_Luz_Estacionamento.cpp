//====================================================================================  
//                   DEPENDÊNCIAS  
//====================================================================================  
 
#include "Sistema_Luz_Estacionamento.h" 
#include "DEBUG_MODE.h" 

#include "Controle_Luz_Estacionamento.h"
#include "Gerencia_Luz_Estacionamento.h"
#include "Leitura_Botao.h"
 
//====================================================================================  
//                   VARIÁVEIS INTERNAS  
//====================================================================================  

//====================================================================================  
//                   SETUP  
//====================================================================================  
 
 void LUZ_ESTACIONAMENTO_BOOT() 
{ 
        debugSetupArquivo();
        debugSetupDivisao(COR::AMARELO, "SISTEMA LUZ ESTACIONAMENTO INICIALIZANDO", COR::MAGENTA, "=", 45);
        
    Leitura_Botao_Inicializa();

    Controle_Luz_Estacionamento_Inicializa();

    Gerencia_Luz_Estacionamento_Inicializa();
} 
 
//====================================================================================  
//                   LOOP  
//====================================================================================  
 
 void LUZ_ESTACIONAMENTO_LOOP() 
{ 
 
    Controle_Luz_Estacionamento_Atualiza();

    Gerencia_Luz_Estacionamento_Atualiza();
 

} 
 
