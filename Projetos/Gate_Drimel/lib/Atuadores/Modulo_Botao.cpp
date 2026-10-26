//====================================================================================  
//                   DEPENDÊNCIAS  
//====================================================================================  
 
#include "Modulo_Botao.h" 
#include "DEBUG_MODE.h" 

#include "Mapa_Pinos_Arduino.h"
 
//====================================================================================  
//                   VARIÁVEIS INTERNAS  
//====================================================================================  
 
static constexpr uint8_t botaoPortaoLuzEstacionamento = pino(ARDUINO_CONEXAO::BOTAO::PORTAO_LUZ_ESTACIONAMENTO);
 
//====================================================================================  
//                   SETUP  
//====================================================================================  
 
 void Modulo_Botao_Inicializa() 
{ 
 
 pinMode (botaoPortaoLuzEstacionamento, INPUT_PULLUP);
 
} 
 
//====================================================================================  
//                   LOOP  
//====================================================================================  
 
 bool Modulo_Botao_Esta_Pressionado()
{ 
    return digitalRead(botaoPortaoLuzEstacionamento) == LOW;
} 
 
