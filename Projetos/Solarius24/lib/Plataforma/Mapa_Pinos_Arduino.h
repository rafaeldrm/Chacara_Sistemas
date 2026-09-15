//====================================================================================
//                   DEPENDÊNCIAS
//====================================================================================

#pragma once         // Garante que o arquivo seja lido uma única vez durante a compilação
#include <Arduino.h> // Biblioteca base do Arduino (tipos, funções padrão, etc)

//====================================================================================
//                   PLANTA ELÉTRICA DIGITAL
//====================================================================================

//====================================================================================
//                           RELÉS
//====================================================================================

/*
Responsáveis por acionar cargas de potência.
Todos os relés são controlados via nível lógico digital.

OBS:
Alguns relés possuem lógica invertida (LOW = ligado).
Isso é tratado em ModuloReles.cpp, NÃO aqui.
*/

namespace ARDUINO_CONEXAO {
    
    enum class RELE : uint8_t
     {
            FONTE_USINA_30A          = 12, // Fonte auxiliar principal (30A) --  Modulo_Reles.h
            FONTE_USINA_60A          = 11, // Fonte futura (expansão somente para a casa nova) --  A DEFINIR sei lá quando
            COOLER_MPPT              = 10,    // Ventilação do controlador MPPT --  Modulo_Reles.h
            INVERSOR                 = 9,       // Inversor 220V 4500W --  Modulo_Reles.h
            GELADEIRA                = 8,      // Alimentação da geladeira --  Modulo_Reles.h
            SERVO_MOTOR              = 7     // Disjuntor DC - Energiza regulador do servo (MP1584) --  Modulo_Reles.h
    };


//====================================================================================
//                         BOTÕES / JUMPERS
//====================================================================================

/*
Entradas físicas para controle manual ou manutenção.
Usados como modo seguro de intervenção humana.
Aciona o Display que fica normalmente apagado (economizando energia, e evitando desgastes desnecessários)
*/

    enum class BOTAO : uint8_t
     {
        USINA_URGENCIA              = 22,    // Possível botão físico de emergência --  Controle_Carga_Urgencia.h
        //Ativa o Display Oled, mas funciona por 2 minutos máximo, depois precisa reapertar esse botão                    
        ACIONA_DISPLAY              = 26  //   -- CRIAR O ARQUIVO
    };

//====================================================================================
//                           SENSORES ANALÓGICOS
//====================================================================================

/*
Sensores conectados às portas analógicas do Arduino Mega.
Utilizados para leitura de sinais variáveis (0–5V).
*/

enum class SENSOR_ANALOGICO : uint8_t
 {
           REDE_ELETRICA_ZMPT101B  = A1, // Sensor AC da rede Tensão (V) --  Driver_ZMPT101B.h
           MAQUINA_LAVAR_SCT013    = A3   // Sensor de Corrente (A) --  Driver_SCT013.h               
 };
//====================================================================================
//                         SENSORES DIGITAIS
//====================================================================================
enum class SENSOR_DIGITAL : uint8_t
{
           TEMPERATURA_MPPT_DHT11 = A0  // Sensor DHT11 (É SENSOR DIGITAL) --  Driver_DHT11.h
};

enum class DIAGNOSTICO : uint8_t
{
           LUZ_DIAGNOSTICO = 13, // Led da placa Arduino (pino 13) --  Led_Status.h
           PULSO_555_RESET = 27 //NE555P para Desligar o Arduino por 5 segundos e Religar, via Relé
};
//====================================================================================
//                           LORA (SPI)
//====================================================================================

/*
Pinos dedicados ao módulo LoRa SX1278.
Utiliza comunicação SPI (hardware do Mega).
*/
enum class LORA : uint8_t 
{
            DIO0 = 2, // interrupção do rádio -- Driver_LoRa.h
            RST  = 6,  // Reset do módulo -- Driver_LoRa.h
            SS   = 53  // Chip Select (SPI padrão do Mega) -- Driver_LoRa.h
};
//====================================================================================
//                          MOTOR SERVO
//====================================================================================

/*
Controle mecânico do disjuntor DC (Sistema Calamidade).
*/

enum class SERVO : uint8_t 
{
                MG996R = 5  // -- Executa_Disjuntor_Servo_Motor_MG996R.h
};
//====================================================================================
//                         ENDEREÇOS I2C (SENSORES)
//====================================================================================

/*
Endereços fixos no barramento I2C.

IMPORTANTE:
Caso algum sensor seja trocado ou jumper alterado,
apenas este arquivo deve ser modificado.
por enquanto catalogarei somente os arduinos que tenho disponível comigo
Arduino Mega 2560 - SDA (20) / SCL (21)
Arduino UNO R3 - SDA (A4) / SCL (A5)
Arduino Nano - SDA (A4) / SCL (A5)
Arduino Due - SDA (20) / SCL (21)
Arduino Pro Micro - SDA (2)  SCL (3)
*/
enum class I2C_ENDERECO : uint8_t
{
    INA226            = 0x40, // Monitor principal de tensão     -- Driver_INA226.h / .cpp
    INA3221_BANCADA_1 = 0x42, // Jumper A0 com SDA               -- Driver_INA3221.h / .cpp 
    INA3221_BANCADA_2 = 0x43, // Jumper A0 com SCL               -- Driver_INA3221.h / .cpp
    DISPLAY_OLED_096  = 0x3C  // Display OLED                    -- Driver_OLED_096.h / .cpp
};

}
//====================================================================================
//                         HELPERS DE CONVERSÃO DE PINOS
//====================================================================================


template<typename T>
constexpr uint8_t pino(T v) {
    return static_cast<uint8_t>(v);
}