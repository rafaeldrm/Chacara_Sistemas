#pragma once
#include <Arduino.h>

//=========================== TIPO ===========================//

enum class TipoRecuperacao : uint8_t
{
    NENHUMA,
    LOGICA,
    WATCHDOG,
    RESET_FISICO
};

//=========================== INTERFACE ===========================//

void Recuperacao_Sistema_Inicializa();

bool Recuperacao_Sistema_Tentar_Logica();

void Recuperacao_Sistema_Forcar_Watchdog();

void Recuperacao_Sistema_Reset_Fisico_555();

TipoRecuperacao Recuperacao_Sistema_Obter_Ultimo_Reset();