//-------------------------- DEPENDÊNCIAS ----------------------------------//

#pragma once
#include <Arduino.h>
#include "Leitura_Baterias.h"
#include "Estados_Modulos.h"

//-------------------------- DADOS ------------------------------------------//

struct StatusDiagnosticoINA3221
{
    SENSOR::Estado estado;

    int32_t ultima_celula[4];

    uint16_t contador_erros;
    uint8_t  contador_travado;

    bool erro_lora_enviado;
    bool mostrar_erro_oled;

    bool sensor_online;
    bool erro_sensor;
};

//-------------------------- FUNÇÕES PÚBLICAS -------------------------------//

void Diagnostico_INA3221_Inicializa();

void Diagnostico_INA3221_Atualiza(const PackBaterias& leitura);

//-------------------------- ACESSO AOS DADOS -------------------------------//

const StatusDiagnosticoINA3221& Diagnostico_INA3221_ObterStatus();

void Diagnostico_INA3221_ResetarErros();
















