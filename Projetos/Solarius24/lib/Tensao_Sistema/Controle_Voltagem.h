//-------------------------- DEPENDÊNCIAS ----------------------------------//

#pragma once
#include <Arduino.h>

//-------------------------- FUNÇÕES PÚBLICAS -------------------------------//

 void Controle_Voltagem_Inicializa();

 bool Controle_Voltagem_Atualizar_Tensao();

uint16_t Controle_Voltagem_Obter_Tensao();

bool Controle_Voltagem_Tem_Dado();

void Controle_Voltagem_Normal();

void Controle_Voltagem_Auxiliar();

void Controle_Voltagem_Emergencial();

void Controle_Voltagem_Critico();

void Controle_Voltagem_Recuperando_Carga();

void Controle_Voltagem_Recuperado();

void Controle_Voltagem_Fim_Automatico();

void Controle_Voltagem_Automatico();

void Controle_Voltagem_Calamidade_Inicio();

void Controle_Voltagem_Calamidade_Completo();

void Controle_Voltagem_Tentativa_Recuperar_Sistema();