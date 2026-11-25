//-------------------------- DEPENDÊNCIAS ----------------------------------//

#pragma once
#include <Arduino.h>

//-------------------------- DADOS ------------------------------------------//

//-------------------------- FUNÇÕES PÚBLICAS -------------------------------//

void Controle_Tranco_Bomba_Casa_Inicializa();

void Controle_Tranco_Bomba_Casa_Atualiza();

void Controle_Tranco_Bomba_Casa_Iniciando_Monitoramento_Helice();

void Controle_Tranco_Bomba_Casa_Travada();
 
void Controle_Tranco_Bomba_Casa_Primeira_Tentativa_Tranco();

void Controle_Tranco_Bomba_Casa_Segunda_Tentativa_Tranco();

void Controle_Tranco_Bomba_Casa_Sensor_Ir_Desativado();

void Controle_Tranco_Bomba_Casa_Liberada();

//-------------------------- ACESSO AOS DADOS -------------------------------//