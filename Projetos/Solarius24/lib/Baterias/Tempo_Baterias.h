//-------------------------- DEPENDÊNCIAS ----------------------------------//

#pragma once
#include <Arduino.h>

//-------------------------- FUNÇÕES PÚBLICAS -------------------------------//

void Tempo_Baterias_Inicializa();

void Tempo_Baterias_Atualiza();

// Controle direto do sensor (usado internamente)
bool Tempo_Baterias_PermiteLeitura();