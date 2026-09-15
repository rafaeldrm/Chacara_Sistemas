#pragma once
#include <Arduino.h>
#include "Estado_Globais.h"

//====================================================================================
//                   MÁQUINA DE ESTADOS
//====================================================================================

ESTADO::URGENCIA::MODO Controle_Carga_Urgencia_Logica_Normal();

ESTADO::URGENCIA::MODO Controle_Carga_Urgencia_Logica_Automatico();

ESTADO::URGENCIA::MODO Controle_Carga_Urgencia_Logica_Calamidade();

//====================================================================================
//                   EVENTOS (ENTRADAS EXTERNAS)
//====================================================================================

bool Evento_Urgencia_LoRa();

bool Evento_Urgencia_Disparado();