//-------------------------- DEPENDÊNCIAS ----------------------------------//

#pragma once
#include <Arduino.h>
#include "Estado_Globais.h"

//-------------------------- FUNÇÕES PÚBLICAS -------------------------------//

void Gerencia_Baterias_Inicializa();

void Gerencia_Baterias_Atualiza();

//-------------------------- FUNÇÕES PRIVADAS -------------------------------//

ESTADO::BATERIA::SITUACAO Controle_Baterias_Logica_Normal();
ESTADO::BATERIA::SITUACAO Controle_Baterias_Logica_Observacao();
ESTADO::BATERIA::SITUACAO Controle_Baterias_Logica_Calamidade_Inicio();
ESTADO::BATERIA::SITUACAO Controle_Baterias_Logica_Calamidade_Disjuntor_Desligado();
ESTADO::BATERIA::SITUACAO Controle_Baterias_Logica_Calamidade_Completo();
ESTADO::BATERIA::SITUACAO Controle_Baterias_Logica_Tentativa_Recuperar_Sistema();
ESTADO::BATERIA::SITUACAO Controle_Baterias_Desativar_Modo_Calamidade();