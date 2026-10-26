//-------------------------- DEPENDÊNCIAS ----------------------------------//

#pragma once
#include <Arduino.h>

//-------------------------- FUNÇÕES PÚBLICAS -------------------------------//

 void Leitura_Eco_Hidro_Boias_Inicializa();

 void Leitura_Eco_Hidro_Boias_Atualiza(uint16_t novoMapa);

 bool Leitura_Boias_Esta_Ativa(uint16_t boia);
