#include "Modulo_Reles.h"
#include "Mapa_Pinos_Arduino.h"
#include "DEBUG_MODE.h"

// Cria a instância global já com o pino correto
Modulo_Rele INVERSOR (
    pino(ARDUINO_CONEXAO::RELE::INVERSOR),
     true,
    "INVERSOR"
);

Modulo_Rele GELADEIRA (
    pino(ARDUINO_CONEXAO::RELE::GELADEIRA),
     false,
    "GELADEIRA"
);

Modulo_Rele FONTE_USINA (
    pino(ARDUINO_CONEXAO::RELE::FONTE_USINA_30A),
    false,
    "FONTE_USINA"
);

Modulo_Rele COOLER_MPPT (
    pino(ARDUINO_CONEXAO::RELE::COOLER_MPPT),
    false,
    "COOLER_MPPT"
);

Modulo_Rele SERVO_MOTOR (
    pino(ARDUINO_CONEXAO::RELE::SERVO_MOTOR),
    true,
    "SERVO_MOTOR"
);

//====================================================================================
//                   CONSTRUTOR
//====================================================================================

Modulo_Rele::Modulo_Rele(uint8_t pino, bool invertido, const char* nome)
    : _pino(pino), _invertido(invertido), _estado(false), _nome(nome) {}

void Modulo_Rele::Setup() {
    pinMode(_pino, OUTPUT);
    // Estado inicial seguro (desligado)
    digitalWrite(_pino, _invertido ? HIGH : LOW);
    debugVariavel("Rele : ", _nome, NEGRITO, AMARELO, VERDE);
    debugVariavel("Arduino : ", _pino, NEGRITO, AMARELO, VERDE);
    debugVariavel("Posição : ", _invertido, NEGRITO, AMARELO, VERDE);
    debugSetupTexto("DRIVER :: Rele Inicializado --- ", LINHA | LINHA);debugSetupTempo();
}

void Modulo_Rele::Ligar() {
    if (_estado) return;
    debugVariavel("Ligando Relé: ", _nome, NEGRITO, VERDE, AMARELO);
    digitalWrite(_pino, _invertido ? LOW : HIGH);
    _estado = true;
}

void Modulo_Rele::Desligar() {
    if (!_estado) return;
    debugVariavel("Desligando Relé: ", _nome, NEGRITO, VERDE, AMARELO);
    digitalWrite(_pino, _invertido ? HIGH : LOW);
    _estado = false;
}

bool Modulo_Rele::Estado() {
    return _estado;
}

void Modulo_Rele::Alternar() {
    _estado ? Desligar() : Ligar();
}

//====================================================================================
//                   SETUP PRINCIPAL
//====================================================================================

void Reles_Setup() {

    debugSetupArquivo();
    debugSetupDivisao(TEXTO_AMARELO, "RELES INICIALIZANDO!!!", TEXTO_CIANO, "MW", 60);
    debugSetupTexto("Iniciando Posicionamentos dos Relés...", VERDE | LINHA);
    
    SERVO_MOTOR.Setup();
    FONTE_USINA.Setup();   
    COOLER_MPPT.Setup();
    INVERSOR.Setup();
    GELADEIRA.Setup();
    
    debugSetupDivisao(TEXTO_AMARELO, "RELES CARREGADOS COM SUCESSO!!!", TEXTO_CIANO, "MW", 60);
}
