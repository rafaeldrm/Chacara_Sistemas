//-------------------------- DEPENDÊNCIAS ----------------------------------//

#pragma once
#include <Arduino.h>


// PINOS 50, 51 e 52 RESERVADOS AO SPI

//-------------------------- PINOS ARDUINO ---------------------------------------//

namespace ARDUINO_CONEXAO {

//====================================================================================
//                   BOIAS
//====================================================================================

    enum class BOIA : uint8_t {

        CASA_MINIMO                = 23,   // Sensor Nível Mínimo Casa (Caixa de 500 litros)
        CASA_MEDIO                 = 25,   // Sensor Nível quase médio da Caixa casa ("IA") (Sistema Residencial)

//========================== TORRE ==================================================

        TORRE_MAXIMO               = 27,   // Sensor Nível Máximo Torre (Caixa 500 litros)

// =================BOIAS RESERVATÓRIO 1 (5000 litros) ==============================

        RESERVA1_MAXIMO            = 29,  //Sensor Nível Máxima 5.000 litros
        RESERVA1_MEDIO             = 31,  //Sensor Nível Média 2.500 +/-
        RESERVA1_MINIMO            = 33, // Sensor Nível Mínima 1.000 +/-

// =================BOIAS RESERVATÓRIO 2 (5000 litros) ==============================

        RESERVA2_MAXIMO            = 35,   //Sensor em 5.000 litros
        RESERVA2_MINIMO            = 37,   //Sensor em mais ou menos em 1.000 litros

// =================BOIAS ENTRADA  (2000 litros) ====================================

        ENTRADA_MAXIMO             = 39,   //Sensor em 2.000 litros
        ENTRADA_MINIMO             = 41,   //Sensor na altura da saída (altura da boca saída)

// =================BOIAS ESTOQUE  (500 litros) ====================================

        ESTOQUE_MAXIMO             = 43,   //Sensor em 510 litros (Aviso Emergencial de Transbordamento)
        ESTOQUE_MINIMO             = 45   //sensor em +/- 150 litros

//=============================== RESERVADOS ============================================
  
                                //   = 47
                                //   = 49
        };

//====================================================================================
//                   RELÉS
//====================================================================================    

 enum class RELES : uint8_t {
        ARMA_RELES_12V              =   12,   //→ alimenta placa de relés
        INVERSOR                    =   11,   //→ liga inversor AC

        LUZ_ESTACIONAMENTO          =   10,   //→ aciona as luzes do estacionamento entrada. Mensagem LoRa, ou botão do portão azul
        LUZ_ACESSO_RESERVATORIO     =    9,   //→ aciona as luzes de acesso para o reservatório. Mensagem LoRa
        
        PORTAO                      =    8,   //→ ativa a placa controladora e motor pra abrir e fechar o portão
        BOMBA_CASA                  =    7,   //→ bomba de transferência Casa ↔ Torre
        BOMBA_RESERVA_12V           =    6    //→ transfere água entre os Reservatórios 1 e 2 (quando há necessidade)
        
        };

//====================================================================================
//                   BOTÕES
//====================================================================================
 
enum class BOTAO : uint8_t {
            PORTAO_LUZ_ESTACIONAMENTO     = 13 //interruptor que fica localizado no portão azul da chácara
            
        };

//====================================================================================
//                   SENSORES
//====================================================================================

enum class SENSOR : uint8_t {

            TEMPERATURA_DHT11              = A4, //58 pino digital
            VOLTAGEM_BATERIA_12V           = A5, //Analógico

//===================SENSOR IR (KY-032)===================================
             
            IR_OUT                    = 5,  // exemplo, pino digital onde vai o OUT do KY-032
            IR_EN                     = 4   // pino para ativar/desativar o sensor (EN)

        };

//====================================================================================
//                   LoRA
//====================================================================================

 enum class LORA : uint8_t {
            DIO0                      = 2,
            RST                       = 3,
            SS                        = 53

        };

//====================================================================================
//                   COMUNICAÇÃO PORTÃO ELETRÔNICO
//====================================================================================

enum class PORTAO_ELETRONICO : uint8_t {
    
        RECEBE_LEITURA_DO_PORTAO              = A0,     //sinal LOW/HIGH que vem do sistema do portão e conecta pelo optoacoplador 4N25
        ENVIA_LEITURA_PARA_PORTAO             = A1,    //sinal  LOW/HIGH que é enviado para o sistema do portão pelo optoacoplador 4N25
        };
}

//====================================================================================
//                   CHAMADAS DOS PINOS NO SISTEMA TODO
//====================================================================================

template<typename T>
constexpr uint8_t pino(T v) {
    return static_cast<uint8_t>(v);
}