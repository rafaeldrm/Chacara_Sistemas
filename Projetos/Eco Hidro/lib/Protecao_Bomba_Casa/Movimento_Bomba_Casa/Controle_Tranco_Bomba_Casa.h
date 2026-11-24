//-------------------------- DEPENDÊNCIAS ----------------------------------//

#pragma once
#include <Arduino.h>

//-------------------------- DADOS ------------------------------------------//

//-------------------------- FUNÇÕES PÚBLICAS -------------------------------//

void Controle_Tranco_Bomba_Inicializa();

void Controle_Tranco_Bomba_Atualiza();

void Controle_Iniciando_Monitoramento_Helice();

void Controle_Bomba_Travada();
 
void Controle_Primeira_Tentativa_Tranco();

void Controle_Segunda_Tentativa_Tranco();

void Controle_Bomba_Sensor_Ir_Desativado();

void Controle_Bomba_Liberada();

//-------------------------- ACESSO AOS DADOS -------------------------------//