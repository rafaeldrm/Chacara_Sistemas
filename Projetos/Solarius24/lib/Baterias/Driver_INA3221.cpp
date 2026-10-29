//====================================================================================
//                   DEPENDÊNCIAS
//====================================================================================

#include "Driver_INA3221.h"
#include "DEBUG_MODE.h"

#include "Mapa_Pinos_Arduino.h"
#include <Wire.h>

//====================================================================================
//                   VARIÁVEIS INTERNAS
//====================================================================================

static constexpr uint8_t SHIFT_BITS = 3;

//====================================================================================
//                   INSTÂNCIAS
//====================================================================================

Driver_INA3221 SENSOR_INA3221_B1(
    pino(ARDUINO_CONEXAO::I2C_ENDERECO::INA3221_BANCADA_1),
    "BANCADA_1"
);

Driver_INA3221 SENSOR_INA3221_B2(
    pino(ARDUINO_CONEXAO::I2C_ENDERECO::INA3221_BANCADA_2),
    "BANCADA_2"
);

//====================================================================================
//                   CONSTRUTOR
//====================================================================================

Driver_INA3221::Driver_INA3221(uint8_t endereco, const char* nome)
    : _endereco(endereco), _nome(nome) {}

//====================================================================================
//                   BASE - ESCRITA
//====================================================================================

bool Driver_INA3221::Escrever_Registro(uint8_t reg, uint16_t valor)
{

    Wire.beginTransmission(_endereco); //inicia comunicacao ao módulo
    Wire.write(reg); //registrador alvo
    Wire.write((uint8_t)(valor >> 8)); //valor de 16bits
    Wire.write((uint8_t)(valor & 0xFF));

    return Wire.endTransmission() == 0; //retorna fim da transmissão I²C 

}

//====================================================================================
//                   BASE - LEITURA
//====================================================================================

bool Driver_INA3221::Ler_Registro(uint8_t reg, int16_t* valor_bruto)
{

    Wire.beginTransmission(_endereco); //
    Wire.write(reg); //registrador alvo

    if (Wire.endTransmission(false) != 0)
        return false;

    if (Wire.requestFrom(_endereco, (uint8_t)2) != 2)
        return false;

    uint32_t t0 = millis();

    while (Wire.available() < 2)
    {
        if (millis() - t0 > 20)
            return false;
    }

    int16_t raw = (int16_t)((Wire.read() << 8) | Wire.read());

    // Ajuste importante do INA3221 (remove bits inválidos)
        raw >>= SHIFT_BITS;

    *valor_bruto = raw; //valor bruto registrado 

    return true; //saída escrita OK

}

//====================================================================================
//                   SETUP
//====================================================================================

bool Driver_INA3221::Inicializa()
{
    debugSetupArquivo();
    debugSetupDivisao(TEXTO_AMARELO, "DRIVER INA3221 Iniciando...", TEXTO_VERDE, "*", 120);
    debugSetupTexto("Configurando pino/endereço do Arduino ao Sensor INA3221", NENHUM | LINHA);
    debugVariavel("INA3221 :: ENDEREÇO :: ", _nome, NEGRITO, VERDE, AMARELO);
    debugCor(TEXTO_VERDE);debugHex(_endereco);debugSetupTexto(" ", NENHUM | LINHA);

    if (!Reset_Software())
    {
        debugErro("INA3221 ");
        debugLoopTexto(" :: ERRO NO RESET", VERMELHO | LINHA);
        debugVariavel("INA3221 :: ", _nome, NEGRITO, AMARELO, VERDE);debugLoopTexto("Endereço ::  ", NENHUM);debugHex(_endereco);
        return false;
    }

    if (!Liga())
    {
        debugErro("INA3221 :: ");
        debugLoopTexto(" :: ERRO AO LIGAR", VERMELHO | LINHA);
        debugVariavel("INA3221 :: ", _nome, NEGRITO, AMARELO, VERDE);debugLoopTexto("Endereço ::  ", NENHUM);debugHex(_endereco);
        return false;
    }

    debugOk("INA3221 :: ");
    debugLoopTexto(" :: LIGADO COM SUCESSO", VERDE_NEON | LINHA);;debugLoopTexto("Endereço ::  ", NENHUM);debugHex(_endereco);

    debugOk("Driver INA3221 inicializado com sucesso!");
    debugCor(TEXTO_NEGRITO);debugSetupTexto("DRIVER -> ", AMARELO); 

    return true;
}

//====================================================================================
//                   RESET
//====================================================================================

bool Driver_INA3221::Reset_Software()
{

    delay(2);
    return Escrever_Registro(REG_CONFIG, CONFIG_RESET);

}

//====================================================================================
//                   LIGA
//====================================================================================

bool Driver_INA3221::Liga()
{

    return Escrever_Registro(REG_CONFIG, CONFIG_OPERACIONAL);

}

//====================================================================================
//                   DESLIGA
//====================================================================================

bool Driver_INA3221::Desliga()
{

    return Escrever_Registro(REG_CONFIG, CONFIG_POWER_DOWN);

}

//====================================================================================
//                   LEITURA CANAL GENÉRICO
//====================================================================================

bool Driver_INA3221::Ler_Canal(uint8_t canal, int16_t* raw)
{
    uint8_t reg = 0;

    switch (canal)
    {
        case 1: reg = REG_BUS_CH1; break;
        case 2: reg = REG_BUS_CH2; break;
        // Canal 3 não utilizado neste projeto

        default:
            debugErro("INA3221 :: ");
            debugVariavel(" ", _nome, NEGRITO, NENHUM, VERMELHO);
            debugLoopTexto(" :: CANAL INVALIDO", VERMELHO | LINHA);
            return false;
    }

    debugVariavel("INA3221 : ", _nome, NEGRITO, NENHUM, VERMELHO);
    debugVariavel(" CANAL : ", canal, NEGRITO, AMARELO, VERDE);

    bool ok = Ler_Registro(reg, raw);

    if (ok)
    {
        debugVariavel("Leitura RAW :", *raw, NEGRITO, AMARELO, LARANJA);
        debugLoopTexto(" OK", VERDE_NEON | LINHA);
    }
    else
    {
        debugLoopTexto(" :: ERRO NA LEITURA", VERMELHO | LINHA);
    }

    return ok;
}