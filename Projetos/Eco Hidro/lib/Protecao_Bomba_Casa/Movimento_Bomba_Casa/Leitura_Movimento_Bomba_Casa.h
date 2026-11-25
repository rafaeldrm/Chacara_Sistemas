//-------------------------- DEPENDÊNCIAS ----------------------------------//

#pragma once
#include <Arduino.h>

//-------------------------- DADOS ------------------------------------------//

//-------------------------- FUNÇÕES PÚBLICAS -------------------------------//

void Leitura_Movimento_Bomba_Casa_Inicializa();

//-------------------------- ACESSO AOS DADOS -------------------------------//

bool Sensor_IR_Bomba_Verificar_Casa_Partida();

uint8_t Leitura_Movimento_Bomba_Casa_Verificar();

void Leitura_Movimento_Bomba_Casa_Desligar();