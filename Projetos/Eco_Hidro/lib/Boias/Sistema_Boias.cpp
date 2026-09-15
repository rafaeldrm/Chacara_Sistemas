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
    debugSetupDivisao(TEXTO_AMARELO,  "SISTEMA BOIAS", TEXTO_MAGENTA, "=", 120);
    
    Driver_Boias_Inicializa();
    
    Leitura_Boias_Inicializa();

    Tempo_Boias_Inicializa();

    Notificacao_Boias_Inicializa();
    
    debugOk("Sistema Boias Iniciado.");
} 
 
//====================================================================================  
//                   LOOP  
//====================================================================================  
 
void BOIAS_LOOP()
{
    if (Tempo_Boias_Processa()) {

       Leitura_Boias_Atualiza();

       Leitura_Boias_Mapa_Completo();

       Notificacao_Boias_Atualiza();
    }
}