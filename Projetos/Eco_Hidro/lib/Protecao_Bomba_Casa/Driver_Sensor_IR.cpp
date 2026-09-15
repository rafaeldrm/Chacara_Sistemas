//====================================================================================
//                   DEPENDÊNCIAS
//====================================================================================

#include "Driver_Sensor_IR.h"
#include "DEBUG_MODE.h"

#include "Mapa_Pinos_Arduino.h"

//====================================================================================
//                   VARIÁVEIS INTERNAS
//====================================================================================

// Cache dos pinos para evitar chamadas de função repetitivas no loop
static const uint8_t pinoOUT = pino(ARDUINO_CONEXAO::SENSOR::IR_OUT);
static const uint8_t pinoEN  = pino(ARDUINO_CONEXAO::SENSOR::IR_EN);

//====================================================================================
//                   SETUP
//====================================================================================

void Driver_Sensor_IR_Inicializa() {
    pinMode(pinoOUT, INPUT); 
    pinMode(pinoEN, OUTPUT); 
    
    // Garante estado inicial desligado
    digitalWrite(pinoEN, LOW); 
}

//====================================================================================
//                   LIGA
//====================================================================================

void Driver_Liga_Sensor_IR() {
    digitalWrite(pinoEN, HIGH);
    delay(100);
    // antes da primeira leitura para estabilização do hardware.
}

//====================================================================================
//                   DESLIGA
//====================================================================================

void Driver_Desliga_Sensor_IR() {
    digitalWrite(pinoEN, LOW);
}

//====================================================================================
//                   LENDO SINAL
//====================================================================================

// Alterado para uint8_t para seguir seu padrão de eficiência
uint8_t Driver_Lendo_Sensor_IR() {
    return (uint8_t)digitalRead(pinoOUT);
}