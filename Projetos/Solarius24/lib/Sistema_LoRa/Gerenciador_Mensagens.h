//-------------------------- DEPENDÊNCIAS ----------------------------------//

#pragma once
#include <Arduino.h>
#include "Protocolo_Chacara.h"

//-------------------------- FUNÇÕES PÚBLICAS -------------------------------//

void LoRa_Gerencia_Inicializa();

void LoRa_Gerencia_Atualiza();

void LoRa_Iniciar_Checklist();

void LoRa_Enviar_Evento(uint8_t codigo, uint16_t valor);