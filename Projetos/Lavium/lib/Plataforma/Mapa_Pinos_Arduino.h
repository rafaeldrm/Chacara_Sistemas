//-------------------------- DEPENDÊNCIAS ----------------------------------//

#pragma once
#include <Arduino.h>

//-------------------------- PINOS ARDUINO ---------------------------------------//

namespace ARDUINO_CONEXAO {

//====================================================================================
//                   NICHOS
//====================================================================================

    enum class REED_SWITCH : uint8_t 
    {
        CUECAS                               = 3, 
        MEIAS_PASSEIO                        = 4,
        MEIAS_CHACARA                        = 5,
        BERMUDAS_CHACARA                     = 6,
        BERMUDAS_PASSEIO                     = 7,
        CAMISETAS_CHACARA                    = 8,
        CAMISETAS_PASSEIO                    = A0,
        TOALHAS                              = A1,
        PIJAMAS                              = A2
    };


    enum class I2C_ENDERECO : uint8_t
{
    DISPLAY_OLED_096                         = 0x3C  // Display OLED  	
                                            //A4 (SDA), 
                                            //A5 (SCL)                   
};

    enum class BOTAO : uint8_t
    {
        CONFIRMA                                = A3, //resistor de 10k e pullup externo
    };

//====================================================================================
//                   LoRA
//====================================================================================

 enum class LORA : uint8_t
 {          
         //   MOSI                      = 11, //lembrete para instalação física, não é usado no código diretamente como os outros pinos
        //    MISO                      = 12, //lembrete para instalação física, não é usado no código diretamente como os outros pinos
        //    SCK                       = 13, //lembrete para instalação física, não é usado no código diretamente como os outros pinos
            DIO0                      = 2,
            RST                       = 9,
            SS                        = 10

        };

}

//====================================================================================
//                   CHAMADAS DOS PINOS NO SISTEMA TODO
//====================================================================================

template<typename T>
constexpr uint8_t pino(T v) {
    return static_cast<uint8_t>(v);
}