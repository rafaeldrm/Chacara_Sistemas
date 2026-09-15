//-------------------------- DEPENDÊNCIAS ----------------------------------//

#pragma once
#include <Arduino.h>

//-------------------------- FUNÇÕES PÚBLICAS -------------------------------//

void Driver_INA226_Inicializa();

bool Driver_INA226_Reset();

bool Driver_INA226_Liga();

bool Driver_INA226_Desliga();

bool Driver_INA226_Calibra();

bool Driver_INA226_Testar();

bool Driver_INA226_Disponivel();

bool Driver_INA226_Reinicializar();

//-------------------------- ACESSO AOS DADOS -------------------------------//

bool EscreverRegistro(uint8_t reg, uint16_t valor);

 bool LerRegistroU16(uint8_t reg, uint16_t* valor);
 
 bool LerRegistroS16(uint8_t reg, int16_t* valor);

bool Driver_INA226_LerMiliVolts(uint32_t* milivolts);

bool Driver_INA226_LerMiliAmperes(int32_t* miliamperes);