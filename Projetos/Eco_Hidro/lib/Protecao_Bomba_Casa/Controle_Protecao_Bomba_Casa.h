//-------------------------- DEPENDÊNCIAS ----------------------------------//

#pragma once
#include <Arduino.h>

//-------------------------- DADOS ------------------------------------------//

//-------------------------- FUNÇÕES PÚBLICAS -------------------------------//

void Controle_Protecao_Bomba_Casa_Inicializa();

bool Controle_Protecao_Bomba_Casa_Atualiza();

void Controle_Protecao_Bomba_Casa_Anti_Travamento();

void Controle_Protecao_Bomba_Casa_Temperatura();

void Controle_Protecao_Bomba_Casa_Tempo_Limite_Anti_Transbordamento();

void Controle_Protecao_Bomba_Casa_Anti_Succao_Ar_Sistema();