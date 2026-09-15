//-------------------------- DEPENDÊNCIAS ----------------------------------//

#pragma once
#include <Arduino.h>

//-------------------------- FUNÇÕES PÚBLICAS -------------------------------//

void Driver_Sensor_IR_Inicializa();

void Driver_Liga_Sensor_IR();

void Driver_Desliga_Sensor_IR();

//-------------------------- ACESSO AOS DADOS -------------------------------//

uint8_t Driver_Lendo_Sensor_IR();