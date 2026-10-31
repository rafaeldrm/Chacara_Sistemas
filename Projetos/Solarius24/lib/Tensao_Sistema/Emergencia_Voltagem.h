#pragma once
#include <Arduino.h>
#include "Diagnostico_INA226.h"

//=========================== DADOS ===========================//

struct EmergenciaVoltagem
{
    bool ativo;
    bool fonte_ligada;
};

//=========================== ESTADOS ===========================//

enum class EstadoEmergenciaVoltagem : uint8_t
{
    NORMAL,
    PROTEGENDO,
    CICLO_LIGADO,
    CICLO_DESLIGADO
};

//=========================== INTERFACE ===========================//

void Emergencia_Voltagem_Inicializa();

void Emergencia_Voltagem_Atualiza(const StatusDiagnosticoINA226& diag);

const EmergenciaVoltagem& Emergencia_Voltagem_ObterEstado();