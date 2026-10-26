//-------------------------- DEPENDÊNCIAS ----------------------------------//

#pragma once
#include <Arduino.h>
#include "Driver_INA3221.h"

//-------------------------- DADOS - BANCADA ------------------------------------------//

struct DadosBancada {
    int32_t voltagemTotal_mV;
    int32_t bateriaInferior_mV;
    int32_t bateriaSuperior_mV;
    int32_t delta_mV;

    bool erro;
};

//-------------------------- DADOS - PACK BATERIAS ------------------------------------------//


struct PackBaterias {
    int32_t celula[4];           // atual
    int32_t celula_anterior[4];  // histórico

    int32_t maior;
    int32_t menor;
    int32_t delta;

    bool erro;

    bool pronta;       // buffer estabilizado
    bool novo_dado;    // novo ciclo completo disponível
};

//-------------------------- FUNÇÕES PÚBLICAS -------------------------------//

void Leitura_Baterias_Inicializa();

void Leitura_Baterias_Loop();

//-------------------------- ACESSO AOS DADOS -------------------------------//

const DadosBancada& Leitura_Baterias_Obter_Bancada1();

const DadosBancada& Leitura_Baterias_Obter_Bancada2();

const PackBaterias& Leitura_Baterias_Obter_Pack();

bool Leitura_Baterias_TemNovoDado();

void Leitura_Baterias_Consumir();
















