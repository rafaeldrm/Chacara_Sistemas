//-------------------------- DEPENDÊNCIAS ----------------------------------//

#pragma once
#include <Arduino.h>

//-------------------------- DADOS ------------------------------------------//

namespace LORA
{
    enum class DIAGNOSTICO : uint8_t
    {

        NORMAL,
        LORA_COM_FALHAS,
        LORA_REINICIANDO,
        RESETANDO_ARDUINO,
        LORA_DESATIVADO,
    };

    void definir(DIAGNOSTICO novo);
    DIAGNOSTICO atualDiagnostico();
    const __FlashStringHelper* texto(DIAGNOSTICO d);

}