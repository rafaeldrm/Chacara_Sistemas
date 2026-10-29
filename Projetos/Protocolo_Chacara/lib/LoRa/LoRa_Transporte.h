//-------------------------- DEPENDÊNCIAS ----------------------------------//

#pragma once
#include <Arduino.h>

#include "Protocolo_Chacara.h"

 //-------------------------- DADOS ------------------------------------------//

 //-------------------------- FUNÇÕES PÚBLICAS -------------------------------//

 void LoRa_Transporte_Inicializa();

bool LoRa_Enviar_Pacote(const LORA::Pacote& pacote);

bool LoRa_Receber_Pacote(LORA::Pacote& pacote);