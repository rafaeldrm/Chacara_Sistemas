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
        //Terminal modo Debug 
        Serial.begin(115200);

        debugSetupArquivo();
        debugSetupDivisao(TEXTO_AMARELO,  "SERIAL UART (TX / RX) ", TEXTO_MAGENTA, "=", 120);
        debugOk("Comunicação Serial Iniciado");
        debugSetupTexto("Baud de 115200 selecionado para melhor performance e estabilidade.", LINHA);

        //Serial 1: DFPlayer Mini (Geralmente usa 9600 bps)
        Serial1.begin(9600);
        debugSetupTexto("Serial 1 (DFPlayer) inicializada em 9600 bps.", LINHA);

        // Serial 2: ESP32 (Geralmente usa 115200 bps)
    //    Serial2.begin(115200);
    //    debugSetupTexto("Serial 2 (ESP32) inicializada em 115200 bps.", LINHA   );

         // Serial 3: ESP12-E (Geralmente usa 115200 bps ou 9600 se for comandos AT)
    //    Serial3.begin(115200);
    //    debugSetupTexto("Serial 3 (ESP12-E) inicializada em 115200 bps.", LINHA);

//-------------------------- I²C BEGIN ---------------------------------------//

//    debugSetupDivisao(TEXTO_AMARELO,  "I2C (SDA / SCL)", TEXTO_MAGENTA, "=", 120);

//    Wire.begin();
    // Define o I²C em 400 kHz.
    // Usado para melhor desempenho com dispositivos compatíveis,
    // respeitando a velocidade máxima suportada pelos dispositivos do barramento.
//    Wire.setClock(400000); 
    
//    debugOk("I2C Barramento Iniciado.");
//    debugSetupTexto("Barramento definido em 400KHz de Clock", LINHA);

//-------------------------- SPI BEGIN ---------------------------------------//

//    debugSetupDivisao(TEXTO_AMARELO,  "SPI (MOSI / MISO / SS / SCK)", TEXTO_MAGENTA, "=", 120);

//    SPI.begin();

 //   debugOk("SPI Comunicação Iniciado.");
 //   debugSetupDivisao(TEXTO_AMARELO,  "COMUNICAÇÃO CONCLUÍDA!!!", TEXTO_MAGENTA, "=", 120);

}