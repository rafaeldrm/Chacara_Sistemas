//-------------------------- DEPENDÊNCIAS ----------------------------------//

#pragma once
#include <Arduino.h>

#include "Protocolo_Chacara.h"

//-------------------------- ACESSO AOS DADOS -------------------------------//

bool Driver_LoRa_Inicializa();

bool Driver_LoRa_Online();

bool Driver_LoRa_Reiniciar();