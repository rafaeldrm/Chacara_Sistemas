#include "Leitura_Botao.h"

// Instâncias globais
Leitura_Botao LEITURA_BOTAO_URGENCIA(BOTAO_URGENCIA);
Leitura_Botao LEITURA_BOTAO_DISPLAY(BOTAO_DISPLAY);

//====================================================================================
// CONSTRUTOR
//====================================================================================

Leitura_Botao::Leitura_Botao(Modulo_Botao& driver)
    : _driver(driver)
{
}

//====================================================================================
// LOOP
//====================================================================================

void Leitura_Botao::Atualizar()
{
    _estadoAtual = _driver.EstaPressionado();

    // Detecta borda de subida (clique)
    if (_estadoAtual && !_estadoAnterior)
    {
        _cliqueDetectado = true;
    }

    _estadoAnterior = _estadoAtual;
}

//====================================================================================
// EVENTO
//====================================================================================

bool Leitura_Botao::FoiPressionado()
{
    if (_cliqueDetectado)
    {
        _cliqueDetectado = false;
        return true;
    }

    return false;
}