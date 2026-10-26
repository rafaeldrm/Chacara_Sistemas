//-------------------------- DEPENDÊNCIAS ----------------------------------//

#pragma once
#include <Arduino.h>

//-------------------------- DADOS ------------------------------------------//

//-------------------------- FUNÇÕES PÚBLICAS -------------------------------//

void Driver_DHT11_Inicializa(); 

//-------------------------- ACESSO AOS DADOS -------------------------------//

bool Driver_DHT11_Aguarda_Nivel(bool nivel, uint16_t timeout);

bool Driver_DHT11_Ler(uint8_t &temperatura, uint8_t &umidade); 