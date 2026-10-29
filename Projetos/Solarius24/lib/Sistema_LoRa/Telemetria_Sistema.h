//-------------------------- DEPENDÊNCIAS ----------------------------------//

#pragma once
#include <Arduino.h>
#include "Protocolo_Chacara.h"

//-------------------------- FUNÇÕES PÚBLICAS -------------------------------//

void LoRa_Enviar_Telemetria(uint8_t codigo, uint16_t valor);