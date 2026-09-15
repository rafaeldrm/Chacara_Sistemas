#pragma once
#include <Arduino.h>
#include "Estado_Globais.h"

enum VOLTAGEM_RELES : uint8_t
    {
        PARADO,
        AGUARDANDO,
        LIGAR_INVERSOR,
        ESPERA,
        LIGAR_GELADEIRA,
        FINALIZADO
    };

void ACOES_Voltagem_Aplicar_Saida(ESTADO::VOLTAGEM::NIVEL estado);

void ACOES_Rede_Aplicar_Saida(ESTADO::REDE::STATUS estado);



void SISTEMA_ACOES_LOOP();