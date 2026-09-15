#pragma once
#include <Arduino.h>
#include "Leitura_Voltagem.h"
#include "Estados_Modulos.h"

//=========================== DADOS ===========================//

struct StatusDiagnosticoINA226
{
    SENSOR::Estado estado;
    SENSOR::Estado estado_anterior;


    uint16_t ultima_tensao;

    uint16_t contador_erros;
    uint8_t  contador_travado;

    bool erro_lora_enviado;
    bool mostrar_erro_oled;

    bool sensor_online;
    bool erro_sensor;
    
    bool solicitar_recuperacao;

     bool evento_recuperado;
     bool evento_erro_novo;

};

//=========================== INTERFACE ===========================//

void Diagnostico_INA226_Inicializa();

void Diagnostico_INA226_Atualiza(const DadosLeituraVoltagem& leitura);

const StatusDiagnosticoINA226& Diagnostico_INA226_ObterStatus();