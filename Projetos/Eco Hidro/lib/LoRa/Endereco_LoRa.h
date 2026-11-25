//-------------------------- DEPENDÊNCIAS ----------------------------------//

#pragma once
#include <Arduino.h>

#include "Protocolo_Chacara.h"

//--------------------------- DADOS -----------------------------------------//

// Endereços configuráveis — altere aqui para mudar o sistema
static constexpr LORA::SISTEMA ENDERECO_LOCAL = LORA::SISTEMA::ECO_HIDRO;   // Remetente
static constexpr LORA::SISTEMA ENDERECO_DESTINO_PADRAO = LORA::SISTEMA::DRIMEL_SYSTEM; // Destino padrão

//-------------------------- FUNÇÕES PÚBLICAS -------------------------------//

void Endereco_LoRa_Inicializa();

// BOOT
bool Endereco_LoRa_PedirBoot();
bool Endereco_LoRa_Operacional();

// ENVIO
bool Endereco_LoRa_Montar(uint8_t codigo, uint16_t valor, LORA::Pacote& pacote);
void Endereco_LoRa_Mensagem(uint8_t codigo, uint16_t valor);

// RECEBIMENTO
bool Endereco_LoRa_Receber(LORA::Pacote& pacote, bool* repetida = nullptr);