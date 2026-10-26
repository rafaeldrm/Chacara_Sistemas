//====================================================================================
//                   DEPENDÊNCIAS
//====================================================================================

#include "Driver_ZMPT101B.h"
#include "Mapa_Pinos_Arduino.h"
#include "DEBUG_MODE.h"

//====================================================================================
//                   VARIÁVEIS INTERNAS
//====================================================================================

 static constexpr uint8_t PINO_ZMPT101B = pino(ARDUINO_CONEXAO::SENSOR_ANALOGICO::REDE_ELETRICA_ZMPT101B);

//====================================================================================
//                   SETUP
//====================================================================================

void Driver_ZMPT101B_Inicializa()
 {

    pinMode(PINO_ZMPT101B, INPUT);

    debugSetupArquivo();
    debugSetupTexto("DRIVER :: ZMPT101B :: INICIALIZANDO :: ", NEGRITO | VERDE | LINHA);
    debugVariavel("ZMPT101B :: PINO ARDUINO :: ", PINO_ZMPT101B, NEGRITO, VERDE, AMARELO);
    debugOk("Driver ZMPT101B inicializado com sucesso!");

}

//====================================================================================
//                   LOOP LEITURA ADC COM PROTEÇÃO
//====================================================================================

uint16_t Driver_ZMPT101B_Ler()
{

    return analogRead(PINO_ZMPT101B);
    
}