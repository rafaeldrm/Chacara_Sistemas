//====================================================================================  
//                   DEPENDÊNCIAS  
//====================================================================================  
 
#include "Sistema_Boias.h" 
#include "DEBUG_MODE.h" 

#include "Driver_Boias.h" 
#include "Leitura_Boias.h"
#include "Tempo_Boias.h"
#include "Notificacao_Boias.h"
 
//====================================================================================  
//                   SETUP  
//====================================================================================  
 
 void BOIAS_BOOT()
{ 
    debugSetupArquivo();
    debugSetupDivisao(COR::AMARELO, "SISTEMA BOIAS", COR::MAGENTA, "=", 45);
        
    Driver_Boias_Inicializa();
    
    Leitura_Boias_Inicializa();

    Leitura_Boias_Atualiza(); //cria o primeiro mapa das boias

    Tempo_Boias_Inicializa();

    Notificacao_Boias_Inicializa();
    
    debugOk("Sistema Boias Concluído!!!.");
} 
 
//====================================================================================  
//                   LOOP  
//====================================================================================  
 
void BOIAS_LOOP()
{
    if (Tempo_Boias_Processa()) {
        debugLoopTexto("BOIAS_LOOP, rodando funções internas", LINHA);

       Leitura_Boias_Atualiza();

       Leitura_Boias_Visual_Binario(Leitura_Boias_Mapa_Completo());
       
       Notificacao_Boias_Atualiza();
       
    }
}