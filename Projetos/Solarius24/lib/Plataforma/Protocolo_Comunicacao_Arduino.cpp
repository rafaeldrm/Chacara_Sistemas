//====================================================================================
//                   DEPENDÊNCIAS
//====================================================================================

#include "Protocolo_Comunicacao_Arduino.h"
#include "DEBUG_MODE.h"

#include <SPI.h>
#include <Wire.h>


//====================================================================================
//                   SETUP
//====================================================================================

void COMUNICACAO_SETUP()
{
//-------------------------- SERIAL BEGIN ---------------------------------------//

        Serial.begin(115200);

        debugSetupArquivo();
        debugSetupDivisao(TEXTO_AMARELO,  "SERIAL UART (TX / RX) ", TEXTO_MAGENTA, "=", 120);
        debugOk("Comunicação Serial Iniciada");
        debugSetupTexto("Baud de 115200 selecionado para melhor performance e estabilidade.", LINHA);

//-------------------------- I²C BEGIN ---------------------------------------//

    debugSetupDivisao(TEXTO_AMARELO,  "I2C (SDA / SCL)", TEXTO_MAGENTA, "=", 120);

    Wire.begin();
    // No Mega 2560, às vezes é bom definir o Clock (400kHz é o padrão para INA/OLED)
    Wire.setClock(400000); 
    
    debugOk("I2C Barramento Iniciado.");
    debugSetupTexto("Barramento definido em 400KHz de Clock", LINHA);

//-------------------------- SPI BEGIN ---------------------------------------//

    debugSetupDivisao(TEXTO_AMARELO,  "SPI (MOSI / MISO / SS / SCK)", TEXTO_MAGENTA, "=", 120);

    SPI.begin();

    debugOk("SPI Comunicação Iniciada.");
    debugSetupDivisao(TEXTO_AMARELO,  "COMUNICAÇÃO CONCLUÍDA!!!", TEXTO_MAGENTA, "=", 120);

}