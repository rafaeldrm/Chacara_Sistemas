#pragma once
#include <Arduino.h>

class Modulo_Rele {
private:
    uint8_t _pino;
    bool _invertido;
    bool _estado;
    const char* _nome; // IDENTIFICAÇÃO HUMANA


public:
    Modulo_Rele(uint8_t pino, bool invertido, const char* nome);

    void Setup();
    void Ligar();
    void Desligar();
    bool Estado();
    void Alternar();
};

// Instâncias globais
extern Modulo_Rele INVERSOR;
extern Modulo_Rele GELADEIRA;
extern Modulo_Rele FONTE_USINA;
extern Modulo_Rele COOLER_MPPT;
extern Modulo_Rele SERVO_MOTOR;

// Setup geral
void Reles_Setup();