//-------------------------- DEPENDÊNCIAS ----------------------------------//

#pragma once
#include <Arduino.h>

//-------------------------- DADOS ------------------------------------------//

class Modulo_Rele {
private:
    uint8_t _pino;
    bool _invertido;
    bool _estado;

public:
    Modulo_Rele(uint8_t pino, bool invertido);

    void Inicializa();
    void Ligar();
    void Desligar();
    bool Estado();
    void Alternar();
};

//-------------------------- FUNÇÕES PÚBLICAS -------------------------------//

void Modulo_Reles_Inicializa();

//-------------------------- ACESSO AOS DADOS -------------------------------//

// Instâncias globais
extern Modulo_Rele RELE_ARMA_RELES_12V;
extern Modulo_Rele RELE_INVERSOR;
extern Modulo_Rele RELE_BOMBA_CASA;
extern Modulo_Rele RELE_BOMBA_RESERVA_12V;
extern Modulo_Rele RELE_PORTAO;
extern Modulo_Rele RELE_LUZ_ESTACIONAMENTO;
extern Modulo_Rele RELE_LUZ_ACESSO_RESERVATORIO;