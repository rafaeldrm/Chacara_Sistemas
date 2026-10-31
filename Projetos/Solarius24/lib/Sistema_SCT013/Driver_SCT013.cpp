//====================================================================================
//                   DEPENDÊNCIAS
//====================================================================================

#include "Driver_SCT013.h"
#include "Mapa_Pinos_Arduino.h"
#include "DEBUG_MODE.h"

//====================================================================================
//                   VARIÁVEIS INTERNAS
//====================================================================================

static constexpr uint8_t PINO_SCT013 = (pino(ARDUINO_CONEXAO::SENSOR_ANALOGICO::MAQUINA_LAVAR_SCT013));

//====================================================================================
//                   SETUP SCT013
//====================================================================================

void Driver_SCT013_Inicializa()
{

    pinMode(PINO_SCT013, INPUT);
    debugSetupArquivo();
    debugVariavel("Driver SCT013 :: PINO ARDUINO:: ", PINO_SCT013, NEGRITO, VERDE, AMARELO);
}

//====================================================================================
//                   LEITURA BRUTA ADC DO SCT013
//====================================================================================

  uint16_t Driver_SCT013_Ler_ADC()
{

    analogRead(PINO_SCT013); // leitura de descarte

    return analogRead(PINO_SCT013);

}