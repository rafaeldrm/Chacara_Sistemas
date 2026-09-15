#pragma once
#include <Arduino.h>
#include "Estados_Modulos.h"

//-------------------------- DADOS ------------------------------------------//

struct DadosLeituraVoltagem {

    uint16_t atual;      // Valor atual (escala 0.1V → 12.3V = 123)
    uint16_t anterior;   // Último valor

    uint16_t media;      // Média estabilizada (0.1V)

    bool novo_dado;
    
    LEITURA::Estado estado;  //Enum Leitura de Estados_Modulos.h

};

//-------------------------- FUNÇÕES PÚBLICAS -------------------------------//

bool Leitura_Voltagem_Inicializa();

void Leitura_Voltagem_Atualiza();

bool Leitura_Voltagem_Tem_Novo_Dado();

void Leitura_Voltagem_Consumir();

void Leitura_Voltagem_Valor_Anterior();

//-------------------------- ACESSO AOS DADOS -------------------------------//

const DadosLeituraVoltagem& Leitura_Voltagem_ObterDados();