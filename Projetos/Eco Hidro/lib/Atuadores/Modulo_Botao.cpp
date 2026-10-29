//====================================================================================  
//                   DEPENDÊNCIAS  
//====================================================================================  
 
#include "Modulo_Botao.h" 
#include "DEBUG_MODE.h" 

#include "Mapa_Pinos_Arduino.h"
 
//====================================================================================  
//                   VARIÁVEIS INTERNAS  
//====================================================================================  
 
static constexpr uint8_t botaoPortaoLuzEstacionamento = pino(ARDUINO_CONEXAO::BOTAO::PORTAO_LUZ_ESTACIONAMENTO); //@@ Mapa_Pinos_Arduino.cpp
 
//====================================================================================  
//                   SETUP  
//====================================================================================  
 
 void Modulo_Botao_Inicializa() 
{ 
        debugSetupDivisao(COR::AMARELO, "MÓDULO BOTÃO INICIALIZANDO", COR::MAGENTA, "=", 45);
        
        pinMode (botaoPortaoLuzEstacionamento, INPUT_PULLUP);
 debugVariavel("Botão Aciona Luz Estacionamento : ", botaoPortaoLuzEstacionamento, NEGRITO, VERDE, AMARELO);
} 
 
//====================================================================================  
//                   LOOP  
//====================================================================================  
 
 bool Modulo_Botao_Esta_Pressionado()
{ 
    return digitalRead(botaoPortaoLuzEstacionamento) == LOW;
} 
 
