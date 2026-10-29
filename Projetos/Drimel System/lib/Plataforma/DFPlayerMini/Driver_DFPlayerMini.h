//-------------------------- DEPENDÊNCIAS ----------------------------------//

#pragma once
#include <Arduino.h>

//-------------------------- DADOS ------------------------------------------//

//-------------------------- FUNÇÕES PÚBLICAS -------------------------------//

bool Driver_DFPlayerMini_Inicializa();

//-------------------------- ACESSO AOS DADOS -------------------------------//

bool Driver_DFPlayerMini_TocaFaixa(uint8_t pasta, uint8_t faixa);

bool Driver_DFPlayerMini_Pausa();

bool Driver_DFPlayerMini_Continua();

bool Driver_DFPlayerMini_Para();

void Driver_DFPlayerMini_DefineVolume(uint8_t volume);