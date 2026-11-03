//-------------------------- DEPENDÊNCIAS ----------------------------------//

#pragma once
#include <Arduino.h>

//-------------------------- DADOS ------------------------------------------//

class Interruptor_Nicho {

    uint8_t  _pino;
    bool     portaAberta;
    uint8_t  quantidade_roupa;

};

//-------------------------- FUNÇÕES PÚBLICAS -------------------------------//

 void Nichos_Inicializa();

 void Nichos_Atualiza();


//-------------------------- ACESSO AOS DADOS -------------------------------//

