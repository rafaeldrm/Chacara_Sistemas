//-------------------------- DEPENDÊNCIAS ----------------------------------//

#pragma once
#include <Arduino.h>

#include "Contrato_Boias_Chacara.h"

 //-------------------------- DADOS ------------------------------------------//

 //-------------------------- FUNÇÕES PÚBLICAS -------------------------------//

void Leitura_Boias_Inicializa();

void Leitura_Boias_Atualiza();

void Leitura_Boias_Visual_Binario(uint16_t mapa);

//-------------------------- ACESSO AOS DADOS -------------------------------//

//constrói o 16bits binário de todas as bóias
uint16_t Leitura_Boias_Montar_Mapa_Boias();

bool Leitura_Boias_Esta_Ativa(REGISTRO_BOIAS::BOIA_BIT boia);

// Retorna mapa já estabilizado
uint16_t Leitura_Boias_Mapa_Completo();

