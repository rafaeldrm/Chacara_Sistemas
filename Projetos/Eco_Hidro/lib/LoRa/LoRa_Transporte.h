//-------------------------- DEPENDÊNCIAS ----------------------------------//

#pragma once
#include <Arduino.h>
#include "Protocolo_Chacara.h"

//-------------------------- FUNÇÕES PÚBLICAS -------------------------------//

// Envia a struct inteira de 4 bytes de uma vez só
void LoRa_Enviar_Pacote(const LORA::Pacote& pacote);

//-------------------------- ACESSO AOS DADOS -------------------------------//

// Tenta ler um pacote do rádio. Retorna true se recebeu algo válido.
bool LoRa_Receber_Pacote(LORA::Pacote& pacote);

