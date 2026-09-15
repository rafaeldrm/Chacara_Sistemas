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
//--------------------------------------------------------------------------

Modulo_Rele RELE_ARMA_RELES_12V (
    pino(ARDUINO_CONEXAO::RELES::ARMA_RELES_12V),
     true
);

//--------------------------------------------------------------------------

Modulo_Rele RELE_INVERSOR (
    pino(ARDUINO_CONEXAO::RELES::INVERSOR),
     false
);

//--------------------------------------------------------------------------

Modulo_Rele RELE_BOMBA_CASA (
    pino(ARDUINO_CONEXAO::RELES::BOMBA_CASA),
    false

);

//--------------------------------------------------------------------------

Modulo_Rele RELE_BOMBA_RESERVA_12V (
    pino(ARDUINO_CONEXAO::RELES::BOMBA_RESERVA_12V),
    false

);

//--------------------------------------------------------------------------

Modulo_Rele RELE_PORTAO (
    pino(ARDUINO_CONEXAO::RELES::PORTAO),
    true

);

//--------------------------------------------------------------------------

Modulo_Rele RELE_LUZ_ESTACIONAMENTO (
    pino(ARDUINO_CONEXAO::RELES::LUZ_ESTACIONAMENTO),
    true

);

//--------------------------------------------------------------------------

Modulo_Rele RELE_LUZ_ACESSO_RESERVATORIO (
    pino(ARDUINO_CONEXAO::RELES::LUZ_ACESSO_RESERVATORIO),
    true

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
    debugSetupTexto("ATUADOR :: Rele Inicializado --- ", LINHA | LINHA);debugSetupTempo();
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

void Modulo_Reles_Inicializa() {

    debugSetupArquivo();
    debugSetupDivisao(TEXTO_AMARELO, "RELES INICIALIZANDO!!!", TEXTO_CIANO, "MW", 60);
    debugSetupTexto("Iniciando Posicionamentos dos Relés...", VERDE | LINHA);
    
    debugLoopTexto("Iniciando Relé ARMA_RELES_12V", AMARELO);
    RELE_ARMA_RELES_12V.Inicializa();
    debugLoopTexto("Iniciando Relé INVERSOR", AMARELO);
    RELE_INVERSOR.Inicializa();
    debugLoopTexto("Iniciando Relé BOMBA_CASA", AMARELO);
    RELE_BOMBA_CASA.Inicializa();
    debugLoopTexto("Iniciando Relé BOMBA_RESERVA_12V", AMARELO);
    RELE_BOMBA_RESERVA_12V.Inicializa();
    debugLoopTexto("Iniciando Relé PORTAO", AMARELO);
    RELE_PORTAO.Inicializa();
    debugLoopTexto("Iniciando Relé LUZ_ESTACIONAMENTO", AMARELO);
    RELE_LUZ_ESTACIONAMENTO.Inicializa();
    debugLoopTexto("Iniciando Relé LUZ_ACESSO_RESERVATORIO", AMARELO);
    RELE_LUZ_ACESSO_RESERVATORIO.Inicializa();
    
    debugSetupDivisao(TEXTO_AMARELO, "RELES CARREGADOS COM SUCESSO!!!", TEXTO_CIANO, "MW", 60);
}