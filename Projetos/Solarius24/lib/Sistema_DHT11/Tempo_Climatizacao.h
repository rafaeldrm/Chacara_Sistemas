#pragma once
#include <Arduino.h>

//-------------------------- DADOS ------------------------------------------//

struct EstadoTempoClimatizacao
{
    bool liberado;
};

//-------------------------- FUNÇÕES PÚBLICAS -------------------------------//

void Tempo_Climatizacao_Inicializa();
void Tempo_Climatizacao_Atualiza();

//-------------------------- ACESSO AOS DADOS -------------------------------//

const EstadoTempoClimatizacao& Tempo_Climatizacao_ObterEstado();