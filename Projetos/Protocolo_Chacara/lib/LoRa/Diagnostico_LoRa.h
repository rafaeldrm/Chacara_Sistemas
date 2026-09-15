//-------------------------- DEPENDÊNCIAS ----------------------------------//

#pragma once
#include <Arduino.h>

//-------------------------- FUNÇÕES PÚBLICAS -------------------------------//

void LoRa_Diagnostico_Inicializa();

void LoRa_Diagnostico_Atualiza();

// Registro de eventos
void LoRa_Diagnostico_RegistrarEnvio(bool sucesso);

void LoRa_Diagnostico_RegistrarRecepcao(bool sucesso);

//-------------------------- ACESSO AOS DADOS -------------------------------//

bool     LoRa_Diagnostico_EstaOnline();

uint16_t LoRa_Diagnostico_FalhaEnvio();

uint16_t LoRa_Diagnostico_FalhaRecepcao();

uint32_t LoRa_Diagnostico_TempoSemAtividade();

uint32_t LoRa_Diagnostico_TempoUltimoReinicio();