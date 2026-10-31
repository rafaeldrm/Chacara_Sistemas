#include "Sistema_LoRa.h"

#include "Driver_LoRa.h"
#include "Gerenciador_Mensagens.h"
#include "Diagnostico_LoRa.h"
#include "DEBUG_MODE.h"

void SISTEMA_LORA_BOOT()
{
    debugAviso("LORA: Inicializando...");

    LoRa_Diag_Inicializa();

    if (!Driver_LoRa_Inicializa())
    {
        debugErro("LORA: Falha ao iniciar");
        return;
    }

    LoRa_Gerencia_Inicializa();

    debugOk("LORA: Online");
}

//------------------------------------------------------------------------------

void SISTEMA_LORA_LOOP()
{
    // Comunicação
    LoRa_Gerencia_Atualiza();

    // Diagnóstico
    LoRa_Diag_Atualiza();
}