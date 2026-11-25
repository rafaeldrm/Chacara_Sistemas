#pragma once

#include <Arduino.h>

namespace LORA
{
namespace SEGURANCA
{

    enum class ESTADO_COMUNICACAO : uint8_t
    {
        NAO_SINCRONIZADO = 0,
        AGUARDANDO_BOOT,
        SINCRONIZANDO,
        OPERACIONAL
    };

    class Estado_Comunicacao_LoRa
    {
    public:
        void Resetar();

        ESTADO_COMUNICACAO Obter() const;

        void IniciarBoot();
        void IniciarSincronizacao();
        void TornarOperacional();

        bool PodeTransmitirNormalmente() const;

    private:
        ESTADO_COMUNICACAO estado = ESTADO_COMUNICACAO::NAO_SINCRONIZADO;
    };

}
}