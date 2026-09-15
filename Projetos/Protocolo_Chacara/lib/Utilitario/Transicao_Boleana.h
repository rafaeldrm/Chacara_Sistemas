//-------------------------- DEPENDÊNCIAS ----------------------------------//

#pragma once
#include <Arduino.h>

//-------------------------- DADOS ------------------------------------------//

inline bool MudouEstado(bool atual, bool& anterior)
{
    if (atual == anterior)
        return false;

    anterior = atual;
    return true;
}

/*
@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
                      EXEMPLO
@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@

crie a primeira variável no topo do arquivo

static bool casaMediaAnterior

dentro da função.

  bool estadoAtual =   <<<<<cria uma segunda variável
        !Leitura_Boias_Esta_Ativa(REGISTRO_BOIAS::BOIA_BIT::CASA_MEDIA);

    if (!MudouEstado(estadoAtual, casaMediaAnterior)) <<<<<<< a função aqui da Trava_Boleana
        return;

*/
