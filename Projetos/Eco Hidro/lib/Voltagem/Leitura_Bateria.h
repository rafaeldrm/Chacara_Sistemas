//-------------------------- DEPENDÊNCIAS ----------------------------------// 
 
#pragma once 
#include <Arduino.h>

//-------------------------- DADOS ------------------------------------------//
 
//-------------------------- FUNÇÕES PÚBLICAS -------------------------------// 
 
void Leitura_Bateria_Inicializa();
 
void Leitura_Bateria_Atualiza();

void Leitura_Bateria_Processa_Media();

void Leitura_Bateria_Iniciar_Media();

 //-------------------------- ACESSO AOS DADOS ---------------------------------------//
 
uint16_t Leitura_Bateria_Entrega();

bool Leitura_Bateria_Liberado();

bool Leitura_Bateria_Finalizada();