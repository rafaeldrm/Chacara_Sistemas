#pragma once
#include <Arduino.h>
#include "Estado_Globais.h"

void Gerencia_Carga_Urgencia_Setup();

void Gerencia_Carga_Urgencia_Loop();

ESTADO::URGENCIA::MODO Controle_Carga_Urgencia_Logica_Normal();
ESTADO::URGENCIA::MODO Controle_Carga_Urgencia_Logica_Automatico();
ESTADO::URGENCIA::MODO Controle_Carga_Urgencia_Logica_Calamidade();


