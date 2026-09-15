//====================================================================================
//                   DEPENDÊNCIAS
//====================================================================================

#include "Driver_Boias.h"
#include "DEBUG_MODE.h"

#include "Mapa_Pinos_Arduino.h"

//====================================================================================
//                   VARIÁVEIS INTERNAS
//====================================================================================

// Ordem DEVE bater com o enum BOIA_ID
static constexpr uint8_t pinosBoias[REGISTRO_BOIAS::TOTAL_BOIAS] = {

    [REGISTRO_BOIAS::CASA_MINIMA] = pino(ARDUINO_CONEXAO::BOIA::CASA_MINIMO),
    [REGISTRO_BOIAS::CASA_MEDIA] = pino(ARDUINO_CONEXAO::BOIA::CASA_MEDIO),

    [REGISTRO_BOIAS::TORRE_MAXIMA] = pino(ARDUINO_CONEXAO::BOIA::TORRE_MAXIMO),

    [REGISTRO_BOIAS::RESERVATORIO_1_MAXIMA] = pino(ARDUINO_CONEXAO::BOIA::RESERVA1_MAXIMO),
    [REGISTRO_BOIAS::RESERVATORIO_1_MEDIA] = pino(ARDUINO_CONEXAO::BOIA::RESERVA1_MEDIO),
    [REGISTRO_BOIAS::RESERVATORIO_1_MINIMA] = pino(ARDUINO_CONEXAO::BOIA::RESERVA1_MINIMO),

    [REGISTRO_BOIAS::RESERVATORIO_2_MAXIMA] = pino(ARDUINO_CONEXAO::BOIA::RESERVA2_MAXIMO),
    [REGISTRO_BOIAS::RESERVATORIO_2_MINIMA] = pino(ARDUINO_CONEXAO::BOIA::RESERVA2_MINIMO),

    [REGISTRO_BOIAS::ENTRADA_MAXIMA] = pino(ARDUINO_CONEXAO::BOIA::ENTRADA_MAXIMO),
    [REGISTRO_BOIAS::ENTRADA_MINIMA] = pino(ARDUINO_CONEXAO::BOIA::ENTRADA_MINIMO),

    [REGISTRO_BOIAS::ESTOQUE_MAXIMA] = pino(ARDUINO_CONEXAO::BOIA::ESTOQUE_MAXIMO),
    [REGISTRO_BOIAS::ESTOQUE_MINIMA] = pino(ARDUINO_CONEXAO::BOIA::ESTOQUE_MINIMO)
};

//====================================================================================
//                   SETUP
//====================================================================================

void Driver_Boias_Inicializa() {
    
    debugSetupTexto("Inicializando Driver Boias", NEGRITO | SUBLINHADO | AZUL | LINHA);

    for (uint8_t i = 0; i < REGISTRO_BOIAS::TOTAL_BOIAS; i++) {
        pinMode(pinosBoias[i], INPUT_PULLUP);
        debugVariavel("PINOS:", pinosBoias[i], NEGRITO, VERDE, AMARELO);
       
    }
}

//====================================================================================
//                   FUNÇÃO QUE INFORMA A BOIA ESPECÍFICA
//====================================================================================

bool Driver_Boias_Ler(REGISTRO_BOIAS::BOIA_BIT boia)
{
    return digitalRead(pinosBoias[static_cast<uint8_t>(boia)]) == LOW;
}