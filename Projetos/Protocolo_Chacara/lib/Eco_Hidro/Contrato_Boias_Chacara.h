//-------------------------- DEPENDÊNCIAS ----------------------------------//

#pragma once 
#include <Arduino.h>

//-------------------------- DADOS ------------------------------------------//

namespace REGISTRO_BOIAS {

        // Fonte única de verdade — usada em Driver e Leituras e o Eco Hidro e Drimel System tem acesso

    enum BOIA_BIT : uint16_t { // Índices de 0 a 11

        //-------------------------- CAIXA CASA ------------------------------------------//
        CASA_MINIMA = 0, 
        CASA_MEDIA = 1, 
        //-------------------------- CAIXA TORRE -----------------------------------------//
        TORRE_MAXIMA = 2,
       //-------------------------- RESERVATÓRIO 1 ---------------------------------------//
        RESERVATORIO_1_MAXIMA = 3, 
        RESERVATORIO_1_MEDIA = 4, 
        RESERVATORIO_1_MINIMA = 5,
        //-------------------------- RESERVATÓRIO 2 --------------------------------------//
        RESERVATORIO_2_MAXIMA = 6, 
        RESERVATORIO_2_MINIMA = 7,
        //-------------------------- CAIXA DA ENTRADA ------------------------------------//
        ENTRADA_MAXIMA = 8,
        ENTRADA_MINIMA = 9,
        //-------------------------- CAIXA ESTOQUE ---------------------------------------//
        ESTOQUE_MAXIMA = 10, 
        ESTOQUE_MINIMA = 11,

        //------------------------- Proteção limite auto atualizável --------------------
        // Marcador interno — NÃO É UMA BOIA. Não passe para Driver_Boias_Ler().
        _TOTAL_BOIAS
    };

    constexpr uint16_t TOTAL_BOIAS  = _TOTAL_BOIAS;
}