#pragma once
#include <Arduino.h> //biblioteca Arduino para C++
#include "Estado_Globais.h"

uint16_t Diagnostico_RamLivre();

void Monitorar_RAM_Atualiza();

void Watchdog_Sistema_Inicializa();

void Watchdog_Sistema_Atualiza();

// Salva na memória não volátil se estamos em calamidade (1) ou normal (0)

void EEPROM_Inicializa();

void Salvar_Estagio_Calamidade_EEPROM(ESTADO::BATERIA::SITUACAO estagio);





