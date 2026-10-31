#pragma once
#include <Arduino.h>

//=========================== FONTE ===========================//

enum class FonteVoltagem : uint8_t
{
    INA226,
    INA3221,
    INDEFINIDO
};

//=========================== DADOS ===========================//

struct DadosVoltagemSistema
{
    uint16_t tensao_mV;
    FonteVoltagem fonte;
    bool valido;
};

//=========================== INTERFACE ===========================//

void Decisor_Voltagem_Inicializa();

void Decisor_Voltagem_Atualiza();

const DadosVoltagemSistema& Decisor_Voltagem_Obter();