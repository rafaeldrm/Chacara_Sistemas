//-------------------------- DEPENDÊNCIAS ----------------------------------//

#pragma once
#include <Arduino.h>
#include "Contrato_Boias_Chacara.h"

//-------------------------- DADOS ------------------------------------------//

//-------------------------- FUNÇÕES PÚBLICAS -------------------------------//

void Driver_Boias_Inicializa();

//-------------------------- ACESSO AOS DADOS -------------------------------//

// Função utilitária para leitura individual
bool Driver_Boias_Ler(REGISTRO_BOIAS::BOIA_BIT boia);