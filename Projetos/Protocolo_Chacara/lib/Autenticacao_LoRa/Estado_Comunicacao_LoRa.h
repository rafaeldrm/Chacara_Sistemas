#pragma once

#include <Arduino.h>

namespace LORA
{
namespace AUTENTICACAO
{

    enum class EstadoComunicacao : uint8_t
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

        EstadoComunicacao Obter() const;

        void IniciarBoot();
        void IniciarSincronizacao();
        void TornarOperacional();

        bool PodeTransmitirNormalmente() const;

    private:
        EstadoComunicacao estado = EstadoComunicacao::NAO_SINCRONIZADO;
    };

}
}