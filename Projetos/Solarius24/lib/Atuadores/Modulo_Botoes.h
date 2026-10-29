#pragma once
#include <Arduino.h>

class Modulo_Botao {
private:
    uint8_t _pino;

    const char* _nome; // IDENTIFICAÇÃO HUMANA

public:
    Modulo_Botao(uint8_t pino,  const char* nome);

    void Setup();

    bool EstaPressionado(); // leitura bruta
};

// Instância global
extern Modulo_Botao BOTAO_URGENCIA;
extern Modulo_Botao BOTAO_DISPLAY;



