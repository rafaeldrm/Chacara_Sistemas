#pragma once
#include <Arduino.h>

struct EstadoTempoTelemetriaMaquinaLavarRoupas
{
    bool liberado;
};

void Tempo_Climatizacao_Inicializa();
void Tempo_Climatizacao_Atualiza();

const EstadoTempoTelemetriaMaquinaLavarRoupas& Tempo_Telemetria_MAquina_Lavar_Roupas_ObterEstado();