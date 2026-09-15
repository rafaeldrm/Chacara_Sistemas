#pragma once
#include <Arduino.h>
#include "Modulo_Botoes.h"

class Leitura_Botao {
private:
    Modulo_Botao& _driver;

    bool _estadoAtual = false;
    bool _estadoAnterior = false;
    bool _cliqueDetectado = false;

public:
    Leitura_Botao(Modulo_Botao& driver);

    void Atualizar();
    bool FoiPressionado();
};

extern Leitura_Botao LEITURA_BOTAO_URGENCIA;
extern Leitura_Botao LEITURA_BOTAO_DISPLAY;