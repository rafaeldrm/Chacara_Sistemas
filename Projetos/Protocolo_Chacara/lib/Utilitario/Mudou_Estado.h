//-------------------------- DEPENDÊNCIAS ----------------------------------//

#pragma once
#include <Arduino.h>

//-------------------------- DADOS ------------------------------------------//

namespace TRAVA_BOLEANA 
{

    inline bool MudouEstado(bool leitura, bool& ultimaLeitura)
    {
        if (leitura == ultimaLeitura)
            return false;

        ultimaLeitura = leitura;   // atualiza a memória com a leitura nova
        return true;
    }
}

/*
@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
                      EXEMPLO
@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@

EXEMPLO DE USO:

// 1) No topo do arquivo (estado inicial):
static bool casaMediaAnterior = false;

// 2) No boot (sincroniza o estado real antes de começar):
void Sistema_Casa_Torre_Boot() {
    casaMediaAnterior = Leitura_Boias_Esta_Ativa(CASA_MEDIA);
}

// 3) No loop:
void Sistema_Casa_Torre_Loop() {
    bool estadoAtual = Leitura_Boias_Esta_Ativa(CASA_MEDIA);

    if (!TRAVA_BOLEANA::MudouEstado(estadoAtual, casaMediaAnterior))
        return;  // sem mudança → não faz nada
}
*/
