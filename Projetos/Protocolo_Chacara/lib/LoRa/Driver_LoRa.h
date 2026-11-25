//-------------------------- DEPENDÊNCIAS ----------------------------------//

#pragma once
#include <Arduino.h>

//-------------------------- DADOS ------------------------------------------//

//-------------------------- FUNÇÕES PÚBLICAS -------------------------------//

bool Driver_LoRa_Inicializa();

bool Driver_LoRa_Reiniciar();

//-------------------------- ACESSO AOS DADOS -------------------------------//

bool Driver_LoRa_Online();

uint32_t Driver_LoRa_Aleatorio32();