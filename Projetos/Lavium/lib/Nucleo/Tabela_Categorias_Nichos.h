//-------------------------- DEPENDÊNCIAS ----------------------------------//

#pragma once
#include <Arduino.h>

//====================================================================================
//                   TABELA DOS NICHOS
//====================================================================================

enum class NICHO_CATEGORIA : uint8_t
{
    CUECAS                 = 0,
    MEIAS_PASSEIO          = 1,
    MEIAS_CHACARA          = 2,
    BERMUDAS_CHACARA       = 3,
    BERMUDAS_PASSEIO       = 4,
    CAMISETAS_CHACARA      = 5,
    CAMISETAS_PASSEIO      = 6,
    TOALHAS                = 7,
    PIJAMAS                = 8,

    TOTAL_CATEGORIAS 
};

//====================================================================================
//                   CONFIGURAÇÃO DOS NICHOS
//====================================================================================

struct ConfiguracaoNicho
{
    uint8_t limiteMaximo;
    const char* nome; // Texto formatado para o Display OLED (max ~16-18 caracteres)
};

//------------- LIMITE MÁXIMO E NOME POR NICHO ------------------------------

constexpr ConfiguracaoNicho configuracao[] =
{
    {25, "Cuecas"},             // CUECAS
    {12, "Meias Passeio"},      // MEIAS_PASSEIO (PAR)
    {12, "Meias Chacara"},      // MEIAS_CHACARA (PAR)
    {5,  "Bermudas Chacara"},     // BERMUDAS_CHACARA
    {5,  "Bermudas Passeio"},     // BERMUDAS_PASSEIO
    {10, "Camisetas Chacara"},    // CAMISETAS_CHACARA
    {8,  "Camisetas Passeio"},    // CAMISETAS_PASSEIO
    {3,  "Toalhas"},            // TOALHAS
    {5,  "Pijamas"}             // PIJAMAS
};

static_assert(
    sizeof(configuracao) / sizeof(configuracao[0]) ==
    static_cast<uint8_t>(NICHO_CATEGORIA::TOTAL_CATEGORIAS),
    "configuracao[] precisa ter uma entrada por categoria"
);