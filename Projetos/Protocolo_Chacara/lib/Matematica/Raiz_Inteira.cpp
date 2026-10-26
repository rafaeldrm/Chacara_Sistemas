//====================================================================================
//                   DEPENDÊNCIAS
//====================================================================================

#include "Raiz_Inteira.h"

//====================================================================================
//                   RAIZ INTEIRA
//====================================================================================
// Calcula a raiz quadrada inteira (arredonda para baixo).
//
// ENTRADA: uint32_t (0 a 4.294.967.295)
// SAÍDA:   uint16_t (0 a 65.535)
//
// SEGURANÇA: o resultado sempre cabe em uint16_t para qualquer
//            entrada válida (raiz de UINT32_MAX é 65.535).
//            Não use valores fora do range de uint32_t.

// Não usa float nem a função sqrt(). Mais leve e rápido no Arduino.
// Exemplo: Raiz_Inteira(25) retorna 5 | Raiz_Inteira(26) retorna 5

uint16_t Raiz_Inteira(uint32_t numero)
{

    uint32_t raiz = 0;
    uint32_t bit  = 1UL << 30;          // maior bit de potência de 4 que cabe em 32 bits

    // Ajusta o bit inicial para não ser maior que o número
    while (bit > numero) //enquanto o “bit” for maior que o número: divide por 4 (>> 2)
    {
        bit >>= 2; 
    }

    while (bit != 0) //aqui começa o cálculo da raiz
    {
        if (numero >= raiz + bit)  //“dá pra encaixar esse pedaço na raiz?”
        {
            numero -= raiz + bit;
            raiz = (raiz >> 1) + bit;
        }
        else
        {
            raiz >>= 1;  // divide por 2
        }

        bit >>= 2; //divide por 4 - vai pro próximo nível (mais fino)
    }

    return static_cast<uint16_t>(raiz);  //resultado pronto
}