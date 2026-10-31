#pragma once
#include <Arduino.h>
#include "Leitura_Temperatura.h"
#include "Estados_Modulos.h"

//-------------------------- DADOS ------------------------------------------//

struct StatusDiagnosticoDHT11
{
    SENSOR::Estado estado;

    uint16_t contador_erros;
    uint8_t  contador_travado;

    uint16_t ultima_temperatura_valida;
    uint16_t ultima_umidade_valida;

    bool solicitar_reset;

    bool erro_lora_enviado;
    bool mostrar_erro_oled;
};

//-------------------------- FUNÇÕES PÚBLICAS -------------------------------//

void Diagnostico_DHT11_Inicializa();

void Diagnostico_DHT11_Atualiza(const DadosLeituraDHT11 &leitura);

//-------------------------- ACESSO ----------------------------------------//

const StatusDiagnosticoDHT11& Diagnostico_DHT11_ObterStatus();