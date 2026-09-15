#pragma once

#include <Arduino.h>


//====================================================================================
//                   CONFIGURAÇÃO
//====================================================================================

#define DEBUG_ON      0
#define DEBUG_BOOT    0
#define DEBUG_SYSTEM  0


//====================================================================================
//                   CONSTANTES
//====================================================================================

#if DEBUG_ON

static constexpr uint16_t TEMPO_BOOT    = 50;
static constexpr uint16_t TEMPO_LOOP    = 300;
static constexpr uint16_t TEMPO_LEITURA = 1000;

#endif

//====================================================================================
//                   FORMATACAO ANSI
//====================================================================================

#define TEXTO_RESETA       F("\033[0m")

#define TEXTO_NEGRITO      F("\033[1m")
#define TEXTO_SUBLINHADO   F("\033[4m")
#define TEXTO_INVERSO      F("\033[7m")
#define TEXTO_ITALICO      F("\033[3m")

#define TEXTO_VERMELHO     F("\033[31m")
#define TEXTO_VERDE        F("\033[32m")
#define TEXTO_AMARELO      F("\033[33m")
#define TEXTO_AZUL         F("\033[34m")
#define TEXTO_MAGENTA      F("\033[35m")
#define TEXTO_CIANO        F("\033[36m")
#define TEXTO_VERDE_NEON   F("\033[38;5;46m")
#define TEXTO_LARANJA      F("\033[38;5;208m")
#define TEXTO_ROXO         F("\033[38;5;129m")


//====================================================================================
//                   OPERAÇÕES DE DEBUG
//====================================================================================

#if DEBUG_ON

enum DebugOp : uint16_t
{
    NENHUM      = 0,

    // Estilos
    NEGRITO     = 1 << 0,
    ITALICO     = 1 << 1,
    SUBLINHADO  = 1 << 2,

    // Cores
    VERMELHO    = 1 << 3,
    VERDE       = 1 << 4,
    AMARELO     = 1 << 5,
    AZUL        = 1 << 6,
    CIANO       = 1 << 7,
    MAGENTA     = 1 << 8,
    VERDE_NEON  = 1 << 9,
    LARANJA     = 1 << 10,
    ROXO        = 1 << 11,

    // Comportamento
    LINHA       = 1 << 12
};

#endif


//====================================================================================
//                   TEXTO
//====================================================================================

#if DEBUG_ON

void debugTexto(
    const __FlashStringHelper* mensagem,
    uint16_t operacao = NENHUM
);

void debugTextoDigitando(
    const __FlashStringHelper* mensagem
);


//====================================================================================
//                   TEXTO + VARIÁVEL
//====================================================================================

template<typename T>
void debugTextoVariavel(
    const __FlashStringHelper* mensagem,
    T valor,
    uint16_t estilo = NENHUM,
    uint16_t corMensagem = NENHUM,
    uint16_t corVariavel = NENHUM,
    uint16_t operacao = NENHUM
)
{
    if (estilo & NEGRITO)
        Serial.print(F("\033[1m"));

    if (estilo & ITALICO)
        Serial.print(F("\033[3m"));

    if (estilo & SUBLINHADO)
        Serial.print(F("\033[4m"));


    if (corMensagem & VERMELHO)
        Serial.print(F("\033[31m"));

    else if (corMensagem & VERDE)
        Serial.print(F("\033[32m"));

    else if (corMensagem & AMARELO)
        Serial.print(F("\033[33m"));

    else if (corMensagem & AZUL)
        Serial.print(F("\033[34m"));

    else if (corMensagem & MAGENTA)
        Serial.print(F("\033[35m"));

    else if (corMensagem & CIANO)
        Serial.print(F("\033[36m"));

    else if (corMensagem & VERDE_NEON)
        Serial.print(F("\033[38;5;46m"));

    else if (corMensagem & LARANJA)
        Serial.print(F("\033[38;5;208m"));

    else if (corMensagem & ROXO)
        Serial.print(F("\033[38;5;129m"));


    Serial.print(mensagem);


    if (corVariavel & VERMELHO)
        Serial.print(F("\033[31m"));

    else if (corVariavel & VERDE)
        Serial.print(F("\033[32m"));

    else if (corVariavel & AMARELO)
        Serial.print(F("\033[33m"));

    else if (corVariavel & AZUL)
        Serial.print(F("\033[34m"));

    else if (corVariavel & MAGENTA)
        Serial.print(F("\033[35m"));

    else if (corVariavel & CIANO)
        Serial.print(F("\033[36m"));

    else if (corVariavel & VERDE_NEON)
        Serial.print(F("\033[38;5;46m"));

    else if (corVariavel & LARANJA)
        Serial.print(F("\033[38;5;208m"));

    else if (corVariavel & ROXO)
        Serial.print(F("\033[38;5;129m"));


    Serial.print(valor);

    Serial.print(F("\033[0m"));


    if (operacao & LINHA)
        Serial.println();
}

#endif


//====================================================================================
//                   MACROS COMPATÍVEIS
//====================================================================================

//====================================================================================
//                   MACROS COMPATIVEIS
//====================================================================================

#if DEBUG_ON

#define debugVariavel(msg, valor, estilo, corMsg, corVar) \
    do { \
        debugTextoVariavel(F(msg), valor, estilo, corMsg, corVar); \
    } while (0)

#else

#define debugSetupTempo()

#define debugSetupTexto(msg, op)

#define debugSetupDivisao(cor, mensagem, cor2, caracter, quantidade)

#define debugSetupArquivo()

#define debugLoopTexto(msg, op)

#define debugLoopTempo()

#define debugHex(valor)

#define debugErro(msg)

#define debugOk(msg)

#define debugAviso(msg)

#define debugVariavel(msg, valor, estilo, corMsg, corVar)

#define debugCor(cor)

#endif


//====================================================================================
//                   INICIALIZAÇÃO
//====================================================================================

void DEBUG_MODE();