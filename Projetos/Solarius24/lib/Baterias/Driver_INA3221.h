//-------------------------- DEPENDÊNCIAS ----------------------------------//

#pragma once
#include <Arduino.h>

//-------------------------- CLASSE ---------------------------------------//

class Driver_INA3221 {

private:

    uint8_t _endereco;

    const char* _nome; // IDENTIFICAÇÃO HUMANA

    //================ REGISTRADORES =================//
    static constexpr uint8_t REG_CONFIG    = 0x00;
    static constexpr uint8_t REG_BUS_CH1   = 0x02;
    static constexpr uint8_t REG_BUS_CH2   = 0x04;
    // Canal 3 ignorado

    //================ CONFIGURAÇÕES =================//
    static constexpr uint16_t CONFIG_RESET       = 0x8000;
    static constexpr uint16_t CONFIG_OPERACIONAL = 0x7127;
    static constexpr uint16_t CONFIG_POWER_DOWN  = 0x7000;

    //================ BASE =================//
    bool Escrever_Registro(uint8_t reg, uint16_t valor);
    bool Ler_Registro(uint8_t reg, int16_t* valor_bruto);

public:

    Driver_INA3221(uint8_t endereco, const char* nome);

    //================ SETUP =================//
    bool Inicializa();

    //================ CONTROLE =================//
    bool Reset_Software();
    bool Liga();
    bool Desliga();

    //================ LEITURA =================//
    bool Ler_Canal(uint8_t canal, int16_t* raw);

};

//-------------------------- INSTÂNCIAS GLOBAIS ---------------------------//

extern Driver_INA3221 SENSOR_INA3221_B1;
extern Driver_INA3221 SENSOR_INA3221_B2;