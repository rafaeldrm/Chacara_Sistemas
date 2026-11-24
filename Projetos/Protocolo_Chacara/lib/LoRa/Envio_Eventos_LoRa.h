//-------------------------- DEPENDÊNCIAS ----------------------------------//

#pragma once
#include <Arduino.h>

//-------------------------- DADOS ------------------------------------------//

namespace LORA
{
    enum class ENVIO_EVENTOS : uint8_t
    {
        NENHUM,
        BOOT_PEDINDO,
        BOOT_AGUARDA_RESPOSTA,
        OPERACIONAL,
    };

    void definir(ENVIO_EVENTOS novo);
    ENVIO_EVENTOS atualEnvio();
    const __FlashStringHelper* texto(ENVIO_EVENTOS s);
}

//-------------------------- FUNÇÕES PÚBLICAS -------------------------------//

void Envio_Eventos_LoRa_Inicializa();

void Envio_Eventos_LoRa_Atualiza();

bool LoRa_Enviar_Evento(uint8_t codigo, uint16_t valor);