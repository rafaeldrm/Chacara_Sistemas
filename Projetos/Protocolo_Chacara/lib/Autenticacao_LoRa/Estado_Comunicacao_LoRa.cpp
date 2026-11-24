#include "Estado_Comunicacao_LoRa.h"

namespace LORA
{
namespace SEGURANCA
{

    void Estado_Comunicacao_LoRa::Resetar()
    {
        estado = ESTADO_COMUNICACAO::NAO_SINCRONIZADO;
    }

    ESTADO_COMUNICACAO Estado_Comunicacao_LoRa::Obter() const
    {
        return estado;
    }

    void Estado_Comunicacao_LoRa::IniciarBoot()
    {
        estado = ESTADO_COMUNICACAO::AGUARDANDO_BOOT;
    }

    void Estado_Comunicacao_LoRa::IniciarSincronizacao()
    {
        estado = ESTADO_COMUNICACAO::SINCRONIZANDO;
    }

    void Estado_Comunicacao_LoRa::TornarOperacional()
    {
        estado = ESTADO_COMUNICACAO::OPERACIONAL;
    }

    bool Estado_Comunicacao_LoRa::PodeTransmitirNormalmente() const
    {
        return estado == ESTADO_COMUNICACAO::OPERACIONAL;
    }

}
}