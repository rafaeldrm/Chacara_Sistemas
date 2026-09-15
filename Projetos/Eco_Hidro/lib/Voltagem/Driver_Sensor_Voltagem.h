//-------------------------- DEPENDÊNCIAS ----------------------------------// 
 
#pragma once 
#include <Arduino.h> 
 
 //-------------------------- FUNÇÕES PÚBLICAS -------------------------------// 
 
 void Driver_Sensor_Voltagem_Inicializa();

 void Driver_Sensor_Voltagem_Atualiza();

 //-------------------------- ACESSO AOS DADOS ---------------------------------------//
 
 uint16_t Driver_Sensor_Voltagem_Entrega();