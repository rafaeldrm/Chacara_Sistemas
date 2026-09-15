//-------------------------- DEPENDÊNCIAS ----------------------------------//

#pragma once
#include <Arduino.h>

#include "Protocolo_Chacara.h"

//-------------------------- FUNÇÕES PÚBLICAS -------------------------------//

void Endereco_LoRa_Inicializa();

void Endereco_LoRa_Mensagem(uint8_t codigo, uint16_t valor);