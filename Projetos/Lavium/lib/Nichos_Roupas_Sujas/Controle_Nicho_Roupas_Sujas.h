//-------------------------- DEPENDÊNCIAS ----------------------------------//

#pragma once
#include <Arduino.h>

//====================================================================================
//                   INTERFACE DE CONTROLE DOS NICHOS
//====================================================================================

void Controle_Nicho_Roupas_Sujas_Inicializa();

// Handlers de Estado (Chamados pela FSM no Gerenciador)
void Controle_Nicho_Roupas_Sujas_Nenhum(uint8_t indice);
void Controle_Nicho_Roupas_Sujas_Acessando(uint8_t indice);
void Controle_Nicho_Roupas_Sujas_Contabilizando(uint8_t indice);
void Controle_Nicho_Roupas_Sujas_Pronto_Para_Lavar(uint8_t indice);
void Controle_Nicho_Roupas_Sujas_Resetando(uint8_t indice);
void Controle_Nicho_Roupas_Sujas_Finalizado(uint8_t indice);

// Funções de Consulta (Para o Display OLED e Comunicação)
uint8_t Controle_Nicho_Roupas_Sujas_Obter_Quantidade(uint8_t indice);