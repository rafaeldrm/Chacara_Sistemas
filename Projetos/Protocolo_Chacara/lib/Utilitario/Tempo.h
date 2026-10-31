//-------------------------- DEPENDÊNCIAS ----------------------------------//

#pragma once

#include <Arduino.h>

//-------------------------- FUNÇÕES PÚBLICAS =------------------------------//

//===============================================================================================================
//                   FORMATO DE USO EXEMPLOS:
//       static constexpr uint32_t TEMPO_CONTADOR = TEMPO::HORAS(5) + TEMPO::MINUTOS(10) + TEMPO::SEGUNDOS(40);
//       static constexpr uint32_t TEMPO_TESTE = TEMPO::MINUTOS(25);
//================================================================================================================


namespace TEMPO 
{

    constexpr uint32_t MILLISSEGUNDOS(uint32_t x)
    {
        return x;
    }

    constexpr uint32_t SEGUNDOS(uint32_t x)
    {
        return MILLISSEGUNDOS(x) * 1000UL;
    }

    constexpr uint32_t MINUTOS(uint32_t x)
    {
        return SEGUNDOS(x) * 60UL;
    }

    constexpr uint32_t HORAS(uint32_t x)
    {
         return MINUTOS(x) * 60UL;
    }

    constexpr uint32_t DIAS(uint32_t x)
    {
        return HORAS(x) * 24UL;
    }
}