//-------------------------- DEPENDÊNCIAS ----------------------------------//

#pragma once
#include <Arduino.h>

#include "Estado_Globais.h"

//-------------------------- FUNÇÕES PÚBLICAS -------------------------------//

void Controle_Baterias_Inicializa();

void Controle_Baterias_Atualiza();

void Controle_Baterias_Ativar_Modo_Calamidade();

//-------------------------- FUNÇÕES PRIVADAS -------------------------------//

ESTADO::BATERIA::SITUACAO Controle_Baterias_Logica_Normal();
ESTADO::BATERIA::SITUACAO Controle_Baterias_Logica_Observacao();
ESTADO::BATERIA::SITUACAO Controle_Baterias_Logica_Calamidade_Inicio();
ESTADO::BATERIA::SITUACAO Controle_Baterias_Logica_Calamidade_Disjuntor_Desligado();
ESTADO::BATERIA::SITUACAO Controle_Baterias_Logica_Calamidade_Completo();
ESTADO::BATERIA::SITUACAO Controle_Baterias_Logica_Tentativa_Recuperar_Sistema();
























