//-------------------------- DEPENDÊNCIAS ----------------------------------//

#pragma once
#include <Arduino.h>

//-------------------------- DADOS ------------------------------------------//

struct DadosLeituraDHT11 
{

    uint16_t temperatura_atual;          // atual

    uint16_t umidade_atual; //valor atual

    uint16_t temperatura_media_atual; //media atual

    uint16_t umidade_media_atual; //media atual

    uint16_t temperatura_antiga;   // último valor confiável

    uint16_t umidade_antiga; //último valor confiável

    bool leitura_finalizada; // leitura finalizada
    
};

//-------------------------- FUNÇÕES PÚBLICAS -------------------------------//

void Leitura_Temperatura_Inicializa();

void Leitura_Temperatura_Atualiza();

void Leitura_Temperatura_Reset();

//-------------------------- ACESSO AOS DADOS -------------------------------//

const DadosLeituraDHT11& Leitura_Temperatura_ObterDados();