//-------------------------- DEPENDÊNCIAS ----------------------------------//

#pragma once
#include <Arduino.h>

//-------------------------- FUNÇÕES PÚBLICAS -------------------------------//

void Controle_Baterias_Inicializa();

void Controle_Baterias_Atualiza();

void Controle_Baterias_Normal();

void Controle_Baterias_Observacao();

void Controle_Baterias_Calamidade_Inicio();

void Controle_Baterias_Calamidade_Disjuntor_Desligado();

void Controle_Baterias_Calamidade_Completo();

void Controle_Baterias_Tentativa_Recuperar_Sistema();