//====================================================================================
//                   DEPENDÊNCIAS
//====================================================================================

#include "Protocolo_Comunicacao_Arduino.h"
#include "DEBUG_MODE.h"

#include <SPI.h>
#include <Wire.h>

//====================================================================================
//                   VARIÁVEIS INTERNAS
//====================================================================================

//====================================================================================
//                   SETUP
//====================================================================================

void COMUNICACAO_BOOT()
{
//-------------------------- SERIAL BEGIN ---------------------------------------//

        Serial.begin(115200);

        debugSetupArquivo();
        debugSetupDivisao(COR::AMARELO, "SERIAL UART (TX / RX) ", COR::MAGENTA, "=", 45);
        debugOk("Comunicação Serial Iniciada");
        debugSetupTexto("Baud de 115200 selecionado para melhor performance e estabilidade.", LINHA);

//-------------------------- I²C BEGIN ---------------------------------------//

    debugSetupDivisao(COR::AMARELO, "I2C (SDA / SCL)", COR::MAGENTA, "=", 45);    
    Wire.begin();
    // Define o I²C em 400 kHz.
    // Usado para melhor desempenho com dispositivos compatíveis,
    // respeitando a velocidade máxima suportada pelos dispositivos do barramento.
    Wire.setClock(400000); 
    
    debugOk("I2C Barramento Iniciado.");
    debugSetupTexto("Barramento definido em 400KHz de Clock", LINHA);

//-------------------------- SPI BEGIN ---------------------------------------//

    debugSetupDivisao(COR::AMARELO, "SPI (MOSI / MISO / SS / SCK)", COR::MAGENTA, "=", 45); 
    SPI.begin();
    
     debugOk("SPI Comunicação Iniciada.");
}