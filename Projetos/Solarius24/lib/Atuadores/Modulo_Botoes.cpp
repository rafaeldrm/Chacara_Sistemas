#include "Modulo_Botoes.h"
#include "Mapa_Pinos_Arduino.h"
#include "DEBUG_MODE.h"

// Cria a instância global já com o pino correto
Modulo_Botao BOTAO_URGENCIA(
    pino(ARDUINO_CONEXAO::BOTAO::USINA_URGENCIA),
    "BOTAO_URGENCIA"
);
Modulo_Botao BOTAO_DISPLAY(
    pino(ARDUINO_CONEXAO::BOTAO::ACIONA_DISPLAY),
    "BOTAO_DISPLAY"
);

//====================================================================================
//                   CONSTRUTOR
//====================================================================================
Modulo_Botao::Modulo_Botao(uint8_t pino,  const char* nome)
  :  _pino(pino), _nome(nome) {}

//====================================================================================
//                   SETUP
//====================================================================================
void Modulo_Botao::Setup() {
    pinMode(_pino, INPUT_PULLUP);
    debugVariavel("Botão : ", _nome, NEGRITO, AMARELO, VERDE);
    debugVariavel("Arduino : ", _pino, NEGRITO, AMARELO, VERDE);
    debugSetupTexto("DRIVER :: Botão Inicializado --- ", LINHA);
}

//====================================================================================
//                   LEITURA
//====================================================================================

bool Modulo_Botao::EstaPressionado() {
    return digitalRead(_pino) == LOW;
}