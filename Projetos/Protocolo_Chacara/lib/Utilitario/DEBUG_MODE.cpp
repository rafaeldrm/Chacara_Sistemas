#include "DEBUG_MODE.h"


//====================================================================================
//                   TEXTO
//====================================================================================

#if DEBUG_ON

void debugTexto(
    const __FlashStringHelper* mensagem,
    uint16_t operacao
)
{
    //----------------------------------------------------------------------------
    // ESTILOS
    //----------------------------------------------------------------------------

    if (operacao & NEGRITO)
        Serial.print(F("\033[1m"));

    if (operacao & ITALICO)
        Serial.print(F("\033[3m"));

    if (operacao & SUBLINHADO)
        Serial.print(F("\033[4m"));


    //----------------------------------------------------------------------------
    // COR
    //----------------------------------------------------------------------------

    if (operacao & VERMELHO)
        Serial.print(F("\033[31m"));

    else if (operacao & VERDE)
        Serial.print(F("\033[32m"));

    else if (operacao & AMARELO)
        Serial.print(F("\033[33m"));

    else if (operacao & AZUL)
        Serial.print(F("\033[34m"));

    else if (operacao & MAGENTA)
        Serial.print(F("\033[35m"));

    else if (operacao & CIANO)
        Serial.print(F("\033[36m"));

    else if (operacao & VERDE_NEON)
        Serial.print(F("\033[38;5;46m"));

    else if (operacao & LARANJA)
        Serial.print(F("\033[38;5;208m"));

    else if (operacao & ROXO)
        Serial.print(F("\033[38;5;129m"));


    //----------------------------------------------------------------------------
    // MENSAGEM
    //----------------------------------------------------------------------------

    Serial.print(mensagem);


    //----------------------------------------------------------------------------
    // RESET
    //----------------------------------------------------------------------------

    if (operacao != NENHUM)
        Serial.print(F("\033[0m"));


    //----------------------------------------------------------------------------
    // QUEBRA DE LINHA
    //----------------------------------------------------------------------------

    if (operacao & LINHA)
        Serial.println();
}


//====================================================================================
//                   TEXTO DIGITANDO
//====================================================================================

void debugTextoDigitando(
    const __FlashStringHelper* mensagem
)
{
    const char* ponteiro = (const char*)mensagem;

    while (pgm_read_byte(ponteiro))
    {
        Serial.print((char)pgm_read_byte(ponteiro));

        delay(25);

        ponteiro++;
    }
}

#endif


//====================================================================================
//                   INICIALIZAÇÃO
//====================================================================================

void DEBUG_MODE()
{
    // Futuramente:
    // inicialização do sistema de debug
}