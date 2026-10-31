#include "Estado_Comunicacao_LoRa.h"

namespace LORA
{
namespace AUTENTICACAO
{

    void Estado_Comunicacao_LoRa::Resetar()
    {
        estado = EstadoComunicacao::NAO_SINCRONIZADO;
    }

    EstadoComunicacao Estado_Comunicacao_LoRa::Obter() const
    {
        return estado;
    }

    void Estado_Comunicacao_LoRa::IniciarBoot()
    {
        estado = EstadoComunicacao::AGUARDANDO_BOOT;
    }

    void Estado_Comunicacao_LoRa::IniciarSincronizacao()
    {
        estado = EstadoComunicacao::SINCRONIZANDO;
    }

    void Estado_Comunicacao_LoRa::TornarOperacional()
    {
        estado = EstadoComunicacao::OPERACIONAL;
    }

    bool Estado_Comunicacao_LoRa::PodeTransmitirNormalmente() const
    {
        return estado == EstadoComunicacao::OPERACIONAL;
    }

}
}