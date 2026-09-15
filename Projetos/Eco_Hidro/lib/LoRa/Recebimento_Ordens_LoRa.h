//-------------------------- DEPENDÊNCIAS ----------------------------------//

#pragma once
#include <Arduino.h>

#include "Protocolo_Chacara.h"

//-------------------------- FUNÇÕES PÚBLICAS -------------------------------//

void Recebimento_Ordens_Lora_Inicializa();

bool Recebimento_Ordens_LoRa(const LORA::Pacote& pacote); //interpreta mensagens recebidas do sistema residencial