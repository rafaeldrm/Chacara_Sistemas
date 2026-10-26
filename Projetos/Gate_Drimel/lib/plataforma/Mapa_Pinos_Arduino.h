//-------------------------- DEPENDÊNCIAS ----------------------------------//

#pragma once
#include <Arduino.h>

//-------------------------- PINOS ARDUINO ---------------------------------------//

namespace ARDUINO_CONEXAO {

// ================= RELÉS ======================================

 enum class RELES : uint8_t {

    
        RELE_BOTOEIRA                                       = 8,
        RELE_TRANCA_MAGNETICA                               = 9,
        RELE_FECHO_TRINCO                                   = 10,

 };

//==========================BOTÕES=====================================

enum class BOTAO : uint8_t {

            ACIONAMENTO_ABERTURA_PORTAO             = 22 // botão provisório para acionamento
            
};

//==========================VOLTAGEM LEITURA 12V ======================

enum class SENSOR : uint8_t {

            VOLTAGEM_TRANCA_MAGNETICA           = A5, //Analógico

};

// ================= LORA =============================================

 enum class LORA : uint8_t {
            DIO0                      = 2,
            RST                       = 7,
            SS                        = 53

 };

//====================PORTAO ELETRONICO =================================

enum class PORTAO_ELETRONICO : uint8_t {
    
        RECEBE_PULSO_ECO_HIDRO              = 5,     //sinal que vem do sistema reservatório e conecta pelo optoacoplador 4N25
        ENVIA_PULSO_ECO_HIDRO               = 6    //sinal que é enviado para o sistema reservatório pelo optoacoplador 4N25
};
}

//-------------------------- CHAMADAS EXTERNAS ---------------------------------------//

template<typename T>
constexpr uint8_t pino(T v) {
    return static_cast<uint8_t>(v);
}