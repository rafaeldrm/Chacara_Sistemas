//====================================================================================
//                   DEPENDÊNCIAS
//====================================================================================

#include "Driver_INA226.h"
#include "Mapa_Pinos_Arduino.h"
#include <Wire.h>
#include "DEBUG_MODE.h"

#include "Tempo.h"

//====================================================================================
//                   VARIÁVEIS INTERNAS (ENCAPSULADAS)
//====================================================================================

static constexpr uint32_t TEMPO_RESET = TEMPO::SEGUNDOS(5); // Tempo de reset do barramento I2C (em milissegundos)

     static constexpr uint8_t ENDERECO = pino(ARDUINO_CONEXAO::I2C_ENDERECO::INA226);

    // ================= REGISTRADORES =================
     static constexpr uint8_t REG_CONFIG          = 0x00;
     static constexpr uint8_t REG_BUS_VOLTAGE     = 0x02;
     static constexpr uint8_t REG_CURRENT         = 0x04;
     static constexpr uint8_t REG_CALIB           = 0x05;
     static constexpr uint8_t REG_MANUFACTURER_ID = 0xFE;
     static constexpr uint8_t REG_DIE_ID          = 0xFF;

    // ================= CONFIGURAÇÕES =================
     static constexpr uint16_t CONFIG_RESET       = 0x8000;
     static constexpr uint16_t CONFIG_POWER_DOWN  = 0x4000;
     static constexpr uint16_t CONFIG_OPERACIONAL = 0x4127;

     static constexpr uint16_t CALIB_VALUE        = 5120;

    //================================================================================
    // BASE - ESCRITA
    //================================================================================

    bool Driver_Escrever_Registro(uint8_t reg, uint16_t valor)
    {

        Wire.beginTransmission(ENDERECO);
        Wire.write(reg);
        Wire.write((uint8_t)(valor >> 8));
        Wire.write((uint8_t)(valor & 0xFF));

        return (Wire.endTransmission() == 0);

    }

    //================================================================================
    //                   BASE - LEITURA - Unsigned
    //================================================================================

    bool Driver_Ler_Registro_U16(uint8_t reg, uint16_t* valor)
{
    Wire.beginTransmission(ENDERECO);
    Wire.write(reg);

    if (Wire.endTransmission(false) != 0)
        return false;

    if (Wire.requestFrom(ENDERECO, (uint8_t)2) != 2)
        return false;

    uint8_t msb = Wire.read();
    uint8_t lsb = Wire.read();

    *valor = ((uint16_t)msb << 8) | lsb;

    return true;
}

//====================================================================================
//                   BASE - LEITURA - Signed
//====================================================================================

bool Driver_Ler_Registro_S16(uint8_t reg, int16_t* valor)
{
    uint16_t temp;

    if (!Driver_Ler_Registro_U16(reg, &temp))
        return false;

    *valor = (int16_t)temp;

    return true;
}

//====================================================================================
//                   SETUP
//====================================================================================

void Driver_INA226_Inicializa() 
{
    debugSetupArquivo();
    debugSetupTexto("DRIVER :: INA226 :: INICIALIZANDO :: ", NEGRITO | VERDE | LINHA);

    Wire.setWireTimeout(TEMPO_RESET, true); // 50ms + reset automático

    if (!Driver_INA226_Reset()) debugErro("RESET FALHOU");
    if (!Driver_INA226_Liga()) debugErro("LIGA FALHOU");
    if (!Driver_INA226_Calibra()) debugErro("CALIBRACAO FALHOU");   
    if (!Driver_INA226_Testar()) {
    debugErro("INA226 NAO DETECTADO!");
    return;
}

    debugOk("Driver INA226 inicializado com sucesso!");
    
}

//====================================================================================
//                   RESET
//====================================================================================

bool Driver_INA226_Reset()
{
    if (!Driver_Escrever_Registro(REG_CONFIG, CONFIG_RESET))
        return false;

    delay(2);
    return true;
}

//====================================================================================
//                   LIGA
//====================================================================================

bool Driver_INA226_Liga()
{

    return Driver_Escrever_Registro(REG_CONFIG, CONFIG_OPERACIONAL);

}

//====================================================================================
//                   DESLIGA
//====================================================================================

bool Driver_INA226_Desliga()
{

    return Driver_Escrever_Registro(REG_CONFIG, CONFIG_POWER_DOWN);

}

//====================================================================================
//                   CALIBRA
//====================================================================================

bool Driver_INA226_Calibra()
{

    return Driver_Escrever_Registro(REG_CALIB, CALIB_VALUE);

}

//====================================================================================
//                    TESTE
//====================================================================================

bool Driver_INA226_Testar()
{

    uint16_t dieID;
    uint16_t manID;

    if (!Driver_Ler_Registro_U16(REG_DIE_ID, &dieID)) return false;
    if (!Driver_Ler_Registro_U16(REG_MANUFACTURER_ID, &manID)) return false;

    if (dieID != 0x2260) {
    debugErro("INA226 DIE ID INVALIDO");
    return false;
}

if (manID != 0x5449) {
    debugErro("INA226 FABRICANTE INVALIDO");
    return false;
}

    debugVariavel("DIE ID REGISTRO ::", dieID, NEGRITO, VERDE, LARANJA); 
    debugVariavel("FABRICANTE ID REGISTRO ::", manID, NEGRITO, VERDE, LARANJA);

    debugOk("INA226 OK");
    return true;

}

//====================================================================================
//                   DISPONÍVEL
//====================================================================================

    bool Driver_INA226_Disponivel()
{
    uint16_t id;
    return (Driver_Ler_Registro_U16(REG_DIE_ID, &id) && id == 0x2260);
}

//====================================================================================
//                   RECUPERAÇÃO
//====================================================================================

bool Driver_INA226_Reinicializar()
{
    if (!Driver_INA226_Reset()) return false;
    delay(5);

    if (!Driver_INA226_Liga()) return false;
    delay(2);

    if (!Driver_INA226_Calibra()) return false;
    delay(2);

    return Driver_INA226_Testar();
}

//====================================================================================
//                    LEITURA DE TENSÃO (mV)
//====================================================================================

bool Driver_INA226_Ler_Mili_Volts(uint32_t* milivolts)
{

    uint16_t raw;

if (!Driver_Ler_Registro_U16(REG_BUS_VOLTAGE, &raw))
    return false;

*milivolts = ((uint32_t)raw * 125UL) / 100UL;

return true;

}

//====================================================================================
//                    LEITURA DE CORRENTE (mA)
//====================================================================================

bool Driver_INA226_Ler_Mili_Amperes(int32_t* miliamperes)
{

int16_t raw;

if (!Driver_Ler_Registro_S16(REG_CURRENT, &raw))
    return false;

*miliamperes = (int32_t)raw;

return true;
    
}