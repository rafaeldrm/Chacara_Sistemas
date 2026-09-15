//-------------------------- DEPENDÊNCIAS ----------------------------------//

#pragma once
#include <Arduino.h>

//-------------------------- DADOS ------------------------------------------//

struct RedeStatus 
{
    uint16_t rms;
    uint16_t rms_anterior;

    uint16_t rms_medio;

    uint16_t maximo;
    uint16_t minimo;

    bool tem_rede; 
} __attribute__((packed)); 

//-------------------------- FUNÇÕES PÚBLICAS -------------------------------//

void Leitura_Rede_Estabilizar();

void Leitura_Rede_Inicializa();

void Leitura_Rede_Atualiza();
  
//-------------------------- ACESSO AOS DADOS -------------------------------//

const RedeStatus& Leitura_Rede_ObterStatus();