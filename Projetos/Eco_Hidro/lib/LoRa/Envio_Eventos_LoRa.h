//-------------------------- DEPENDÊNCIAS ----------------------------------//

#pragma once
#include <Arduino.h>

//-------------------------- FUNÇÕES PÚBLICAS -------------------------------//

 void Envio_Eventos_LoRa_Inicializa();

 void Envio_Eventos_LoRa_Atualiza();

//-------------------------- ACESSO AOS DADOS -------------------------------//

bool Envio_Eventos_LoRa_Solicitar(uint8_t codigo, uint16_t valor);

bool Envio_Eventos_LoRa_Ocupado();

bool Envio_Eventos_LoRa_Ultimo_Sucesso();