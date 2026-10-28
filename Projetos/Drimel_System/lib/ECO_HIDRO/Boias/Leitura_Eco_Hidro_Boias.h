//-------------------------- DEPENDÊNCIAS ----------------------------------//

#pragma once
#include <Arduino.h>

//-------------------------- FUNÇÕES PÚBLICAS -------------------------------//

void Leitura_Eco_Hidro_Boias_Inicializa();

void Leitura_Eco_Hidro_Boias_Atualiza(uint16_t novoMapa);

bool Leitura_Eco_Hidro_Boias_Tem_Dado();

bool Leitura_Eco_Hidro_Boias_Esta_Ativa(REGISTRO_BOIAS::BOIA_BIT boia);

uint16_t Leitura_Eco_Hidro_Boias_Mapa_Completo();
