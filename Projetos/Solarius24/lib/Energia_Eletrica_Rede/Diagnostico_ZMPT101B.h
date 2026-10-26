//-------------------------- DEPENDÊNCIAS ----------------------------------//

#pragma once
#include <Arduino.h>
#include "Leitura_Rede.h"
#include "Estados_Modulos.h"

//-------------------------- DADOS ------------------------------------------//

struct StatusDiagnosticoZMPT101B
{
    SENSOR:: Estado estado;
    
    bool tem_rede;

    uint16_t ultimo_rms;

    uint16_t contador_erros;
    uint8_t  contador_travado;

    bool erro_lora_enviado;
    bool mostrar_erro_oled;

    bool sensor_online;
    bool erro_sensor;
};

//-------------------------- FUNÇÕES PÚBLICAS -------------------------------//

void Diagnostico_ZMPT101B_Inicializa();

void Diagnostico_ZMPT101B_Atualiza(const RedeStatus& leitura);

void Diagnostico_ZMPT101B_Resetar_Erros();

//-------------------------- ACESSO AOS DADOS -------------------------------//

const StatusDiagnosticoZMPT101B& Diagnostico_ZMPT101B_ObterStatus();