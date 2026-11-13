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

    [REGISTRO_BOIAS::CASA_MINIMA] = pino(ARDUINO_CONEXAO::BOIA::CASA_MINIMA),
    [REGISTRO_BOIAS::CASA_MEDIA] = pino(ARDUINO_CONEXAO::BOIA::CASA_MEDIA),

    [REGISTRO_BOIAS::TORRE_MAXIMA] = pino(ARDUINO_CONEXAO::BOIA::TORRE_MAXIMA),

    [REGISTRO_BOIAS::RESERVATORIO_1_MAXIMA] = pino(ARDUINO_CONEXAO::BOIA::RESERVATORIO1_MAXIMA),
    [REGISTRO_BOIAS::RESERVATORIO_1_MEDIA] = pino(ARDUINO_CONEXAO::BOIA::RESERVATORIO1_MEDIA),
    [REGISTRO_BOIAS::RESERVATORIO_1_MINIMA] = pino(ARDUINO_CONEXAO::BOIA::RESERVATORIO1_MINIMA),

    [REGISTRO_BOIAS::RESERVATORIO_2_MAXIMA] = pino(ARDUINO_CONEXAO::BOIA::RESERVATORIO2_MAXIMA),
    [REGISTRO_BOIAS::RESERVATORIO_2_MINIMA] = pino(ARDUINO_CONEXAO::BOIA::RESERVATORIO2_MINIMA),

    [REGISTRO_BOIAS::ENTRADA_MAXIMA] = pino(ARDUINO_CONEXAO::BOIA::ENTRADA_MAXIMA),
    [REGISTRO_BOIAS::ENTRADA_MINIMA] = pino(ARDUINO_CONEXAO::BOIA::ENTRADA_MINIMA),

    [REGISTRO_BOIAS::ESTOQUE_MAXIMA] = pino(ARDUINO_CONEXAO::BOIA::ESTOQUE_MAXIMA),
    [REGISTRO_BOIAS::ESTOQUE_MINIMA] = pino(ARDUINO_CONEXAO::BOIA::ESTOQUE_MINIMA)
};

//====================================================================================
//                   SETUP
//====================================================================================

void Driver_Boias_Inicializa() {
    
    debugSetupDivisao(COR::AMARELO, "DRIVER BÓIAS INICIALIZANDO", COR::MAGENTA, "=", 0);
    
    for (uint8_t i = 0; i < REGISTRO_BOIAS::TOTAL_BOIAS; i++) {
        pinMode(pinosBoias[i], INPUT_PULLUP);
        debugVariavel("|| PINOS das Bóias :", pinosBoias[i], NEGRITO, VERDE, AMARELO);
       
    }
}

//====================================================================================
//                   FUNÇÃO QUE INFORMA A BOIA ESPECÍFICA
//====================================================================================

bool Driver_Boias_Ler(REGISTRO_BOIAS::BOIA_BIT boia)
{
    return digitalRead(pinosBoias[static_cast<uint8_t>(boia)]) == LOW;
}