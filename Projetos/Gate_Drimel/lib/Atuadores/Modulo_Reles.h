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

void Reles_Inicializa();

//-------------------------- ACESSO AOS DADOS -------------------------------//

// Instâncias globais
extern Modulo_Rele RELE_BOTOEIRA;
extern Modulo_Rele RELE_TRANCA_MAGNETICA;