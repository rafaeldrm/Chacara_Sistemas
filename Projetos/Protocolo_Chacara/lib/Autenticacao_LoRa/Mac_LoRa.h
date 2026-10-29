#pragma once

#include <Arduino.h>
#include "Chaves_LoRa.h"
#include "Protocolo_Chacara.h"

namespace LORA
{
namespace AUTENTICACAO
{

    // Calcula e valida o MAC (SipHash-2-4, implementação própria e portável).
    // Não conhece pacote, sequência nem estado: recebe bytes e uma chave.

    class Mac_LoRa
    {
    public:
        bool Gerar(
            const uint8_t* dados,
            uint16_t tamanho,
            const Chave& chave,
            uint8_t mac[TAMANHO_MAC_LORA]) const;

        bool Validar(
            const uint8_t* dados,
            uint16_t tamanho,
            const Chave& chave,
            const uint8_t mac[TAMANHO_MAC_LORA]) const;
    };

}
}

void Mac_LoRa_Inicializa();