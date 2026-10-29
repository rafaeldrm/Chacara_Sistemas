//-------------------------- DEPENDÊNCIAS ----------------------------------//

#pragma once
#include <Arduino.h>

//-------------------------- STRUCT ----------------------------------------//

struct DadosCorrenteMaquina {
    uint16_t corrente;
    uint16_t potencia;
    bool novo_dado;
};

//-------------------------- FUNÇÕES PÚBLICAS -------------------------------//

void Leitura_Corrente_Maquina_Loop();

const DadosCorrenteMaquina& Leitura_Corrente_Maquina_Pegar();

void Leitura_Corrente_Maquina_Consumir();