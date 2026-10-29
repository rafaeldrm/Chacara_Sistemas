#include <LoRa.h>
#include "LoRa_Transporte.h"
#include "Driver_LoRa.h"
#include "Diagnostico_LoRa.h"
#include "DEBUG_MODE.h"

static uint8_t ultimoHash = 0;

void LoRa_Enviar_Pacote(const LORA::Pacote& pacote)
{
    if (!Driver_LoRa_Online()) return;

    LoRa.beginPacket();
    LoRa.write((uint8_t*)&pacote, sizeof(LORA::Pacote));

    int status = LoRa.endPacket();

    LoRa_Diag_RegistrarEnvio(status == 1);

    if (status == 1)
    {
        debugOk("LoRa >>> PACOTE ENVIADO");
        LORA::PrintPacote(pacote);
    }
    else
    {
        debugErro("LoRa >>> FALHA ENVIO");
    }
}

//------------------------------------------------------------------------------

bool LoRa_Receber_Pacote(LORA::Pacote& pacote)
{
    if (!Driver_LoRa_Online()) return false;

    int size = LoRa.parsePacket();

    if (size == sizeof(LORA::Pacote))
    {
        LoRa.readBytes((uint8_t*)&pacote, sizeof(LORA::Pacote));

        if (!LORA::ValidarPacote(pacote))
        {
            LoRa_Diag_RegistrarRecepcao(false);
            debugErro("LoRa <<< HASH INVALIDO");

            while (LoRa.available()) LoRa.read();
            return false;
        }

        // 🔥 FILTRO DE DUPLICADO
        if (pacote.hash == ultimoHash)
        {
            debugAviso("LoRa <<< PACOTE REPETIDO IGNORADO");
            return false;
        }

        ultimoHash = pacote.hash;

        LoRa_Diag_RegistrarRecepcao(true);

        debugOk("LoRa <<< PACOTE RECEBIDO");
        LORA::PrintPacote(pacote);
        return true;
    }

    if (size > 0)
    {
        debugErro("LoRa <<< TAMANHO INVALIDO");
        while (LoRa.available()) LoRa.read();
    }

    return false;
}