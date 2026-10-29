//-------------------------- DEPENDÊNCIAS ----------------------------------//

#pragma once
#include <Arduino.h>

//-------------------------- DADOS ------------------------------------------//

namespace LORA
{
    namespace AUTENTICACAO
    {

    constexpr uint8_t TAMANHO_CHAVE_LORA = 16;

    struct Chave
    {
        uint8_t bytes[TAMANHO_CHAVE_LORA];
    };

    // Chave usada exclusivamente pelo par Eco_Hidro <-> Drimel System.
    const Chave& ObterChave();

    }
}

//-------------------------- FUNÇÕES PÚBLICAS -------------------------------//

//-------------------------- ACESSO AOS DADOS -------------------------------//