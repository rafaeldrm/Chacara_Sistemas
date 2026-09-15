//====================================================================================
//                   DEPENDÊNCIAS
//====================================================================================

#include "Sistema_LoRa.h"
#include "DEBUG_MODE.h"

#include "Driver_LoRa.h"
#include "Gerenciador_Mensagens.h"
#include "Diagnostico_LoRa.h"
#include "Recebimento_Ordens_LoRa.h"
#include "Endereco_LoRa.h"
#include "Envio_Eventos_LoRa.h"

//====================================================================================
//                   SETUP
//====================================================================================

void LORA_BOOT()
{
    debugAviso("LORA: Inicializando...");

    if (!Driver_LoRa_Inicializa())
    {
        debugErro("LORA: Falha ao iniciar");
        return;
    }

    LoRa_Diagnostico_Inicializa();

    Endereco_LoRa_Inicializa();

    LoRa_Gerencia_Inicializa();

    Recebimento_Ordens_Lora_Inicializa();

    

    debugOk("LORA: Online");
}

//====================================================================================
//                   LOOP
//====================================================================================

void LORA_LOOP()
{

    LoRa_Gerencia_Atualiza();

    LoRa_Diagnostico_Atualiza();

    Envio_Eventos_LoRa_Atualiza();
    
}