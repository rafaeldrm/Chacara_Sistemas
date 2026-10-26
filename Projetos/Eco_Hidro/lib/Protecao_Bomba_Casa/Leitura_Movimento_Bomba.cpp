//====================================================================================
//                   DEPENDÊNCIAS
//====================================================================================

#include "Leitura_Movimento_Bomba.h"
#include "DEBUG_MODE.h"

#include "Tempo.h"
#include "Cronometro.h"

#include "Driver_Sensor_IR.h"

//====================================================================================
//                   VARIÁVEIS INTERNAS
//====================================================================================

static constexpr uint32_t TEMPO_LEITURA_SENSOR_IR = TEMPO::SEGUNDOS(5);
static CRONOMETRO::Temporizador leituraHeliceMonitoramento;

//====================================================================================
//                   SETUP
//====================================================================================

void Leitura_Movimento_Bomba_Inicializa()
{
        debugSetupDivisao(COR::AMARELO, "LEITURA MOVIMENTO BOMBA INICIALIZANDO", COR::MAGENTA, "=", 45);
}

//====================================================================================
//                   Movimento_Bomba_Verificar
//====================================================================================

uint8_t Leitura_Movimento_Bomba_Verificar()
{
    Driver_Liga_Sensor_IR();

    delay(50);

    uint8_t ultimoEstado = Driver_Lendo_Sensor_IR();
    uint8_t transicoes = 0;

    leituraHeliceMonitoramento.Cronometro_Iniciar();

    while (!leituraHeliceMonitoramento.Cronometro_Dispara_Uma_Vez(
        TEMPO_LEITURA_SENSOR_IR))
    {
        const uint8_t atual = Driver_Lendo_Sensor_IR();

        if (atual != ultimoEstado)
        {
            ++transicoes;
            ultimoEstado = atual;
        }

        delay(40);
    }

    return transicoes;
}

//====================================================================================
//                   MOVIMENTO BOMBA — DESLIGAR SENSOR IR
//====================================================================================

void Leitura_Movimento_Bomba_Desligar()
{
    Driver_Desliga_Sensor_IR();
}