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

void COMUNICACAO_BOOT()
{
//-------------------------- SERIAL BEGIN ---------------------------------------//

        Serial.begin(115200);

        debugSetupArquivo();
        debugSetupDivisao(TEXTO_AMARELO,  "SERIAL UART (TX / RX) ", TEXTO_MAGENTA, "=", 120);
        debugOk("Comunicação Serial Iniciado");
        debugSetupTexto("Baud de 115200 selecionado para melhor performance e estabilidade.", LINHA);

//-------------------------- I²C BEGIN ---------------------------------------//

    debugSetupDivisao(TEXTO_AMARELO,  "I2C (SDA / SCL)", TEXTO_MAGENTA, "=", 120);

    Wire.begin();
    // Define o I²C em 400 kHz.
    // Usado para melhor desempenho com dispositivos compatíveis,
    // respeitando a velocidade máxima suportada pelos dispositivos do barramento.
    Wire.setClock(400000); 
    
    debugOk("I2C Barramento Iniciado.");
    debugSetupTexto("Barramento definido em 400KHz de Clock", LINHA);

//-------------------------- SPI BEGIN ---------------------------------------//

//    debugSetupDivisao(TEXTO_AMARELO,  "SPI (MOSI / MISO / SS / SCK)", TEXTO_MAGENTA, "=", 120);

//    SPI.begin();

 //   debugOk("SPI Comunicação Iniciado.");
 //   debugSetupDivisao(TEXTO_AMARELO,  "COMUNICAÇÃO CONCLUÍDA!!!", TEXTO_MAGENTA, "=", 120);

}