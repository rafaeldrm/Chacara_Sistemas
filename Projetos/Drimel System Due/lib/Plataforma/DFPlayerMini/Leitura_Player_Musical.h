//-------------------------- DEPENDÊNCIAS ----------------------------------//

#pragma once
#include <Arduino.h>

//-------------------------- DADOS ------------------------------------------//

//-------------------------- FUNÇÕES PÚBLICAS -------------------------------//

void Leitura_Player_Musical_Inicializa();

//-------------------------- ACESSO AOS DADOS -------------------------------//

bool Leitura_Player_Musical_Em_Reproducao();

uint8_t Leitura_Player_Musical_Converte_Volume_Para_DFPlayer(uint8_t volumeDrimel);

uint8_t Leitura_Player_Musical_Converte_Volume_Para_Drimel(uint8_t volumeDFPlayer);