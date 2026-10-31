#pragma once
#include <Arduino.h>

void AtualizarValoresDisplay(
    uint16_t temperatura, //DHT11
    uint16_t tensaoAC, //ZMPT101B
    uint16_t tensaoACMax, //ZMPT101B
    uint16_t tensaoACMin, //ZMPT101B 
    uint16_t tensaoBancada1_Bateria1, //INA3221 #1 (Canal 1)
    uint16_t tensaoBancada1_Bateria2, //INA3221 #1 (Canal 2)
    uint16_t tensaoBancada2_Bateria1, //INA3221 #2 (Canal 1)
    uint16_t tensaoBancada2_Bateria2, //INA3221 #2 (Canal 2)
    uint16_t tensaoBateria, //INA226
    uint16_t correnteMaquina //SCT013

);

bool Display_Oled_Novo_Dado();

void Desliga_Display();