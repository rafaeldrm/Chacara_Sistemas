#pragma once
#include <Arduino.h>
#include "Leitura_Corrente_Maquina.h"

void Servico_Telemetria_Maquina_Lava_Roupas_Setup();

void Servico_Telemetria_Maquina_Lava_Roupas_Loop(const DadosCorrenteMaquina& corrente);