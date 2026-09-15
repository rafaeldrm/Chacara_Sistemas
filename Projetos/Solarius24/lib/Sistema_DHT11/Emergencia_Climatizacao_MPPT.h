//-------------------------- DEPENDÊNCIAS ----------------------------------//

#pragma once
#include <Arduino.h>
#include "Diagnostico_DHT11.h"

//-------------------------- DADOS ------------------------------------------//

struct EmergenciaClimatizacao
{
    bool ativo;
    bool cooler_ligado;
};

//-------------------------- ESTADOS ------------------------------------------//

enum class EstadoEmergenciaClimatizacao : uint8_t
{
    NORMAL,
    LIGADO,
    DESLIGADO
};

//-------------------------- FUNÇÕES PÚBLICAS -------------------------------//

void Emergencia_Climatizacao_MPPT_Inicializa();
void Emergencia_Climatizacao_MPPT_Atualiza(const StatusDiagnosticoDHT11& status);

//-------------------------- ACESSO AOS DADOS -------------------------------//

const EmergenciaClimatizacao& Emergencia_Climatizacao_ObterEstado();


