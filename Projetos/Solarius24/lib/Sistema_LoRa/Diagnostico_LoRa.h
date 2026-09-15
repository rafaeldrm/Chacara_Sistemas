#pragma once
#include <Arduino.h>

// Inicialização
void LoRa_Diag_Inicializa();

// Registro de eventos
void LoRa_Diag_RegistrarEnvio(bool sucesso);
void LoRa_Diag_RegistrarRecepcao(bool sucesso);

// Loop
void LoRa_Diag_Atualiza();

// Consultas
bool     LoRa_Diag_EstaOnline();
uint16_t LoRa_Diag_FalhaEnvio();
uint16_t LoRa_Diag_FalhaRecepcao();
uint32_t LoRa_Diag_TempoSemAtividade();