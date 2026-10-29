//-------------------------- DEPENDÊNCIAS ----------------------------------//

#pragma once
#include <Arduino.h>

//-------------------------- DADOS ------------------------------------------//

struct DadosLeituraTemperatura
{
    uint16_t temperatura;   // escala x10 (ex: 650 = 65.0°C)
    uint16_t umidade;       // escala x10
    bool valido;
};

//-------------------------- FUNÇÕES PÚBLICAS -------------------------------//

void Leitura_Temperatura_Inicializa();

void Leitura_Temperatura_Atualiza();


//-------------------------- ACESSO AOS DADOS -------------------------------//

// acesso simples
int16_t Leitura_Temperatura_Obter_Temperatura(); // -1 se inválido

// acesso completo (se precisar)
const DadosLeituraTemperatura& Leitura_Temperatura_Obter_Dados();