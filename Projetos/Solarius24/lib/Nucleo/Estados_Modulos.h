#pragma once
#include <Arduino.h>


namespace SENSOR
{
    enum Estado : uint8_t
    {
        OK,
        INSTAVEL,
        ERRO, 
        INVALIDO
    };
}

namespace LEITURA
{
    enum Estado : uint8_t
    {
        NAO_PRONTA,
        COLETANDO,
        PRONTA
    };
}

namespace TEMPO
{
    enum Estado : uint8_t
    {
        DORMINDO,
        LIBERADO
    };
}

namespace EMERGENCIA
{
    enum Estado : uint8_t
    {
        INATIVO,
        ATIVO
    };
}