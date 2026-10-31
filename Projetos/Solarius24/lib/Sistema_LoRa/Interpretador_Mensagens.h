//-------------------------- DEPENDÊNCIAS ----------------------------------//

#pragma once
#include <Arduino.h>
#include "Protocolo_Chacara.h"


//-------------------------- FUNÇÕES PÚBLICAS -------------------------------//

 bool LoRa_Interpretar(const LORA::Pacote& pacote);