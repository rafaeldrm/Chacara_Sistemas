//====================================================================================
//                   DEPENDÊNCIAS
//====================================================================================

#include "Sistema_LoRa.h"
#include "DEBUG_MODE.h"

#include "Driver_LoRa.h"
#include "Mac_LoRa.h"
#include "Ordem_Recebimento_LoRa.h"
#include "Endereco_LoRa.h"
#include "Envio_Eventos_LoRa.h"

//====================================================================================
//                   VARIÁVEIS INTERNAS
//====================================================================================

static constexpr uint32_t INTERVALO_RECUPERAR_RADIO_MS = 10000;

static bool loraAtivo = false;
static uint32_t proximaTentativaMs = 0;

//====================================================================================
//                   AUXILIARES
//====================================================================================

// Seguro contra a volta do millis() (a cada ~49 dias)
static bool Passou(uint32_t alvo)
{
    return (int32_t)(millis() - alvo) >= 0;
}

//====================================================================================
//                   SOBE AS CAMADAS (DEPOIS QUE O RÁDIO ESTÁ ONLINE)
//====================================================================================

static void IniciarCamadas()
{
    Mac_LoRa_Inicializa();

    Endereco_LoRa_Inicializa();

    Ordem_Recebimento_Lora_Inicializa();

    Envio_Eventos_LoRa_Inicializa();

    loraAtivo = true;

    debugOk("LORA: Online");
}

//====================================================================================
//                   SETUP
//====================================================================================

void LORA_BOOT()
{
    debugSetupArquivo();
    debugSetupDivisao(COR::AMARELO, "SISTEMA LORA INICIALIZANDO", COR::MAGENTA, "=", 45);

    if (!Driver_LoRa_Inicializa())
    {
        debugErro("LORA: Falha ao iniciar (vai tentar de novo)");

        proximaTentativaMs = millis() + INTERVALO_RECUPERAR_RADIO_MS;
        return;
    }

    IniciarCamadas();
}

//====================================================================================
//                   LOOP
//====================================================================================

void LORA_LOOP()
{
    if (!loraAtivo)
    {
        if (Passou(proximaTentativaMs))
        {
            proximaTentativaMs = millis() + INTERVALO_RECUPERAR_RADIO_MS;

            if (Driver_LoRa_Reiniciar())
                IniciarCamadas();
        }

        return;
    }



    Ordem_Recebimento_LoRa_Atualiza();   // escuta o rádio e executa ordens

    Envio_Eventos_LoRa_Atualiza();       // BOOT e fila de mensagens
}