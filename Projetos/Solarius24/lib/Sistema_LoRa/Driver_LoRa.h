//-------------------------- DEPENDÊNCIAS ----------------------------------//

#pragma once
#include <Arduino.h>

//-------------------------- FUNÇÕES PÚBLICAS -------------------------------//

bool Driver_LoRa_Inicializa();


//-------------------------- ACESSO AOS DADOS -------------------------------//

bool Driver_LoRa_Online();

// Ações
bool Driver_LoRa_Enviar(const char* msg);

bool Driver_LoRa_Reiniciar();