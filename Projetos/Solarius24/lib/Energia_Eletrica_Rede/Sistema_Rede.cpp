//====================================================================================
//                   DEPENDÊNCIAS
//====================================================================================

#include "Sistema_Rede.h"

#include "Driver_ZMPT101B.h"
#include "Leitura_Rede.h"
#include "Diagnostico_ZMPT101B.h"
#include "Controle_Rede.h"
#include "Gerencia_Rede.h"
#include "Estados_Sistema_Solarius24.h"
#include "DEBUG_MODE.h"

//====================================================================================
//                   INSTÂNCIAS
//====================================================================================

const StatusDiagnosticoZMPT101B& statusDiag = Diagnostico_ZMPT101B_ObterStatus();


//====================================================================================
//                   SETUP
//====================================================================================

void SISTEMA_REDE_BOOT()
{
    debugSetupArquivo();
    debugSetupTexto("=== SISTEMA REDE BOOT ===", NEGRITO | MAGENTA | LINHA);


    //Driver
    Driver_ZMPT101B_Inicializa();

    // Leitura
    Leitura_Rede_Inicializa();

    // Diagnóstico
    Diagnostico_ZMPT101B_Inicializa();

     // Gerência
    Gerencia_Rede_Inicializa();

    // Controle
    Controle_Rede_Inicializa();

     debugSetupTexto("=== REDE ONLINE ===", NEGRITO | VERDE | LINHA);
}

//====================================================================================
//                   LOOP
//====================================================================================

void SISTEMA_REDE_LOOP()
{

      // Só roda se o sistema de rede estiver ativo
    if (ESTADO::REDE::atual() == REDE::STATUS::DORMINDO)
        return;

        
    // 1. Atualiza Leitura e Diagnóstico
    Leitura_Rede_Atualiza();
    Diagnostico_ZMPT101B_Atualiza(Leitura_Rede_ObterStatus());

    // 2. Chama a Gerência (que por sua vez chama o Controle que finalizamos)
    // O Controle agora é soberano e já sabe lidar com erros do sensor.
    Gerencia_Rede_Atualiza(); 
}