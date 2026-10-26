//====================================================================================
//                   DEPENDÊNCIAS
//====================================================================================

#include "Modulo_Reles.h"
#include "Mapa_Pinos_Arduino.h"
#include "DEBUG_MODE.h"

//====================================================================================
//                   CONSTRUTOR
//====================================================================================

Modulo_Rele::Modulo_Rele(uint8_t pino, bool invertido)
    : _pino(pino), _invertido(invertido), _estado(false) {}
    
// Cria a instância global já com o pino correto
Modulo_Rele RELE_BOTOEIRA (
    pino(ARDUINO_CONEXAO::RELES::RELE_BOTOEIRA),
     true
);

Modulo_Rele RELE_TRANCA_MAGNETICA (
    pino(ARDUINO_CONEXAO::RELES::RELE_TRANCA_MAGNETICA),
     false
);

//====================================================================================
//                   SETUP
//====================================================================================

void Modulo_Rele::Inicializa() {
    pinMode(_pino, OUTPUT);
    // Estado inicial seguro (desligado)
    digitalWrite(_pino, _invertido ? HIGH : LOW);
    _estado = false;
    debugVariavel("Arduino : ", _pino, NEGRITO, AMARELO, VERDE);
    debugVariavel("Posição : ", _invertido?  "INVERTIDO" : "NORMAL", NEGRITO, AMARELO, VERDE);
    debugSetupTexto("DRIVER :: Rele Inicializado --- ", LINHA | LINHA);debugSetupTempo();
}

//====================================================================================
//                   LIGA
//====================================================================================

void Modulo_Rele::Ligar() {
    if (_estado) return;
    digitalWrite(_pino, _invertido ? LOW : HIGH);
    _estado = true;
    debugLoopTexto("Rele Ligado", VERDE);
}

//====================================================================================
//                   DESLIGA
//====================================================================================

void Modulo_Rele::Desligar() {
    if (!_estado) return;
    digitalWrite(_pino, _invertido ? HIGH : LOW);
    _estado = false;
    debugLoopTexto("Rele Desligado", VERDE);
}

//====================================================================================
//                   SITUAÇÃO
//====================================================================================

bool Modulo_Rele::Estado() {
    return _estado;
}

//====================================================================================
//                   ALTERNA LIGADO/DESLIGADO
//====================================================================================

void Modulo_Rele::Alternar() {
    _estado ? Desligar() : Ligar();
}

//====================================================================================
//                   SETUP PRINCIPAL
//====================================================================================

void Reles_Inicializa() {

    debugSetupArquivo();
    debugSetupDivisao(TEXTO_AMARELO, "RELES INICIALIZANDO!!!", TEXTO_CIANO, "MW", 60);
    debugSetupTexto("Iniciando Posicionamentos dos Relés...", VERDE | LINHA);
    
    debugLoopTexto("Iniciando Relé BOTOEIRA", AMARELO);
    RELE_BOTOEIRA.Inicializa();
    debugLoopTexto("Iniciando Relé TRANCA MAGNÉTICA", AMARELO);
    RELE_TRANCA_MAGNETICA.Inicializa();
    
    debugSetupDivisao(TEXTO_AMARELO, "RELES CARREGADOS COM SUCESSO!!!", TEXTO_CIANO, "MW", 60);
}