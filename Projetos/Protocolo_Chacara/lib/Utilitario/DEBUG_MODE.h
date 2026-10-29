/*
 FUNÇÃO DO ARQUIVO:
 ------------------------------------------------------------------------------
 Este arquivo fornece macros e funções para depuração (logs) via Serial.

 Ele permite ativar ou desativar mensagens de debug globalmente ou via hardware.

 OBJETIVO:
 ------------------------------------------------------------------------------
 - Permitir diagnósticos detalhados sem poluir o código de produção.
 - Controlar a verbosidade do sistema através de um Jumper físico.
 - Economizar recursos (memória/processamento) quando DEBUG_ON for 0.

 IMPORTANTE:
 ------------------------------------------------------------------------------
 As macros debugPrint/Println desaparecem do código compilado se DEBUG_ON = 0.

 ARQUIVOS QUE UTILIZAM:
 ------------------------------------------------------------------------------
 Todos os arquivos utilizam
*/

//====================================================================================
//                   DEPENDÊNCIAS
//====================================================================================

#pragma once
#include <Arduino.h>

//====================================================================================
//                   ATIVA/DESATIVA MODO DEBUG
//====================================================================================

#define DEBUG_ON      1     // 1 = DEBUG ATIVO | 0 = DEBUG REMOVIDO, ARDUINO NEM SABE QUE EXISTE SERIAL E DELAYS
#define DEBUG_BOOT    1     // 1 = DEBUG PESADÃO CHEIO DE FIRULA DELAYS, LENTÃO   | 0 = SETUP RODA DIRETÃO, SUAVE, RÁPIDÃO
#define DEBUG_SYSTEM  1     // 1 = DEBUG BÁSICO PARA O LOOP DO SISTEMA, CONFERÊNCIA DE ETAPAS, VARIÁVEIS  | 0 = LOOP LIMPO

//====================================================================================
//                   INFRAESTRUTURA DE DEBUG (só existe se DEBUG_ON = 1)
//====================================================================================

#if DEBUG_ON

//====================================================================================
//                   VARÁVEIS INTERNAS
//====================================================================================

static constexpr uint16_t TEMPO_BOOT    = 20;
static constexpr uint16_t TEMPO_LOOP    = 2000;
static constexpr uint16_t TEMPO_LEITURA = 1000;

//====================================================================================
//                   ESCREVE LETRA A LETRA (EFEITO)
//====================================================================================

inline void debugSetupTextoDigitando(const __FlashStringHelper* msg)
{
    const char* ptr = reinterpret_cast<const char*>(msg);
    char c;
    while ((c = pgm_read_byte(ptr))) {
        Serial.print(c);
        delay(25);
        ptr++;
    }
}

//====================================================================================
//                   FORMATAÇÃO ANSI - DEBUG SERIAL
//====================================================================================

namespace TEXTO
{
    inline constexpr char RESETA[]     PROGMEM = "\033[0m";
    // --------- Estilos --------------
    inline constexpr char NEGRITO[]    PROGMEM = "\033[1m";
    inline constexpr char SUBLINHADO[] PROGMEM = "\033[4m";
    inline constexpr char INVERSO[]    PROGMEM = "\033[7m";
    inline constexpr char ITALICO[]    PROGMEM = "\033[3m";
}

namespace COR
{
    inline constexpr char VERMELHO[]   PROGMEM = "\033[31m";
    inline constexpr char VERDE[]      PROGMEM = "\033[32m";
    inline constexpr char AMARELO[]    PROGMEM = "\033[33m";
    inline constexpr char AZUL[]       PROGMEM = "\033[34m";
    inline constexpr char MAGENTA[]    PROGMEM = "\033[35m";
    inline constexpr char CIANO[]      PROGMEM = "\033[36m";
    inline constexpr char VERDE_NEON[] PROGMEM = "\033[38;5;46m";
    inline constexpr char LARANJA[]    PROGMEM = "\033[38;5;208m";
    inline constexpr char ROXO[]       PROGMEM = "\033[38;5;129m";
}

//====================================================================================
//                   HELPER DE IMPRESSÃO EM FLASH
//====================================================================================

inline void debugPrintCor(const char* flashStr)
{
    Serial.print(reinterpret_cast<const __FlashStringHelper*>(flashStr));
}

//====================================================================================
//                   PERSONALIZA TEXTO, COM COR E PULA LINHA
//====================================================================================

enum DebugOp : uint16_t
{
    NENHUM      = 0,

    // Estilos
    NEGRITO     = 1 << 0,
    ITALICO     = 1 << 1,
    SUBLINHADO  = 1 << 2,

    // Cores (grupo separado)
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
    LINHA       = 1 << 12,  // quebra linha
};

//====================================================================================
//                   MONTANDO TEMPLATE PARA MENSAGENS DE TEXTOS NORMAIS E PARA VARIAVEIS.
//====================================================================================

inline void debugAplicaEstilo(uint16_t op)
{
    if (op & NEGRITO)    debugPrintCor(TEXTO::NEGRITO);
    if (op & ITALICO)    debugPrintCor(TEXTO::ITALICO);
    if (op & SUBLINHADO) debugPrintCor(TEXTO::SUBLINHADO);
}

inline void debugAplicaCor(uint16_t op)
{
    if (op & VERMELHO)        debugPrintCor(COR::VERMELHO);
    else if (op & VERDE)      debugPrintCor(COR::VERDE);
    else if (op & AMARELO)    debugPrintCor(COR::AMARELO);
    else if (op & AZUL)       debugPrintCor(COR::AZUL);
    else if (op & MAGENTA)    debugPrintCor(COR::MAGENTA);
    else if (op & CIANO)      debugPrintCor(COR::CIANO);
    else if (op & VERDE_NEON) debugPrintCor(COR::VERDE_NEON);
    else if (op & LARANJA)    debugPrintCor(COR::LARANJA);
    else if (op & ROXO)       debugPrintCor(COR::ROXO);
}

inline void debugTexto(const __FlashStringHelper* msg, uint16_t op = NENHUM)
{
    debugAplicaEstilo(op);
    debugAplicaCor(op);
    Serial.print(msg);
    if (op != NENHUM) debugPrintCor(TEXTO::RESETA);
    if (op & LINHA)   Serial.println();
}

template<typename T>
inline void debugTextoVariavel(const __FlashStringHelper* msg, T valor,
                               uint16_t estilo = NENHUM,
                               uint16_t corMsg = NENHUM,
                               uint16_t corVar = NENHUM,
                               uint16_t op     = NENHUM)
{
    debugAplicaEstilo(estilo);
    debugAplicaCor(corMsg);
    Serial.print(msg);

    debugAplicaCor(corVar);
    Serial.print(valor);

    debugPrintCor(TEXTO::RESETA);
    Serial.println();
}

#endif //DEBUG_ON

//====================================================================================
//                   BOOT DEBUG
//====================================================================================
// do-while(0) garante que a macro se comporte como uma única instrução

#if DEBUG_ON && DEBUG_BOOT

#define debugSetupTempo() do { delay(TEMPO_BOOT); } while (0)

#define debugSetupTexto(msg, op) \
    do { debugTexto(F(msg), op); debugSetupTempo(); } while (0)

#define debugSetupDivisao(cor, mensagem, cor2, caracter, quantidade)   \
    do {                                                               \
        debugPrintCor(TEXTO::NEGRITO);                                 \
        Serial.println(F(" "));                                        \
        debugPrintCor(cor);                                            \
        Serial.print(F("                         "));                  \
        debugSetupTextoDigitando(F(mensagem));                         \
        Serial.println(F(""));                                         \
        Serial.print(F("                  "));                  \
        debugPrintCor(TEXTO::NEGRITO);                                 \
        debugPrintCor(cor2);                                           \
        for (uint8_t i = 0; i < quantidade; i++) {                     \
            delay(10);                                                 \
            Serial.print(F(caracter));                                 \
        }                                                              \
        Serial.println(F(" "));                                        \
        debugPrintCor(TEXTO::RESETA);                                  \
        debugSetupTempo();                                             \
    } while (0)

#define debugSetupArquivo()                                            \
    do {                                                               \
        debugPrintCor(TEXTO::NEGRITO);                                 \
        debugPrintCor(TEXTO::ITALICO);                                 \
        debugPrintCor(COR::VERDE_NEON);                                \
        Serial.println(F(" "));                                        \
        Serial.println(F(" "));                                        \
        for (uint8_t i = 0; i < 120; i++) {                            \
            delay(10);                                                 \
            Serial.print(F("#"));                                      \
        }                                                              \
        Serial.println(F(""));                                         \
        debugPrintCor(COR::LARANJA);                                   \
        Serial.print(F("                         "));                  \
        debugSetupTextoDigitando(F(__BASE_FILE__));                    \
        Serial.println(F(""));                                         \
        debugPrintCor(TEXTO::NEGRITO);                                 \
        debugPrintCor(COR::VERDE_NEON);                                \
        for (uint8_t i = 0; i < 120; i++) {                            \
            delay(10);                                                 \
            Serial.print(F("#"));                                      \
        }                                                              \
        Serial.println(F(""));                                         \
        debugPrintCor(TEXTO::RESETA);                                  \
        debugSetupTempo();                                             \
    } while (0)

#else

#define debugSetupTempo()
#define debugSetupTexto(msg, op)
#define debugSetupDivisao(cor, mensagem, cor2, caracter, quantidade)
#define debugSetupArquivo()

#endif //DEBUG_BOOT

//====================================================================================
//                   SYSTEM DEBUG
//====================================================================================

#if DEBUG_ON && DEBUG_SYSTEM

#define debugLoopTexto(msg, op) do { debugTexto(F(msg), op); } while (0)

#define debugLoopTempo() do { delay(TEMPO_LOOP); } while (0)

#define debugHex(valor)                     \
    do {                                    \
        Serial.print(F("0x"));              \
        if ((valor) < 0x10) Serial.print('0'); \
        Serial.print((valor), HEX);         \
    } while (0)

#else

#define debugLoopTexto(msg, op)
#define debugLoopTempo()
#define debugHex(valor)

#endif //DEBUG_SYSTEM

//====================================================================================
//                   COMANDOS DEBUG GERAL
//====================================================================================

#if DEBUG_ON

#define debugErro(msg)                                                 \
    do {                                                               \
        debugPrintCor(TEXTO::NEGRITO);                                 \
        debugPrintCor(COR::VERMELHO);                                  \
        Serial.println(F("================================================================== ")); \
        Serial.print(F("[ERRO] "));                                    \
        Serial.println(F(msg));                                        \
        Serial.println(F("================================================================== ")); \
        debugPrintCor(TEXTO::RESETA);                                  \
    } while (0)

#define debugOk(msg)                                                   \
    do {                                                               \
        debugPrintCor(COR::VERDE);                                     \
        debugPrintCor(TEXTO::NEGRITO);                                 \
        Serial.print(F("[OK] "));                                      \
        Serial.print(F(msg));                                          \
        debugPrintCor(TEXTO::RESETA);                                  \
        Serial.println();                                              \
    } while (0)

#define debugAviso(msg)                                                \
    do {                                                               \
        debugPrintCor(TEXTO::NEGRITO);                                 \
        debugPrintCor(COR::AMARELO);                                   \
        Serial.print(F("[AVISO] "));                                   \
        Serial.print(F(msg));                                          \
        debugPrintCor(TEXTO::RESETA);                                  \
        Serial.println();                                              \
    } while (0)

#define debugVariavel(msg, valor, estilo, corMsg, corVar) \
    do { debugTextoVariavel(F(msg), valor, estilo, corMsg, corVar); } while (0)

#define debugCor(cor) do { debugPrintCor(cor); } while (0)

#else

#define debugErro(msg)
#define debugOk(msg)
#define debugAviso(msg)
#define debugVariavel(msg, valor, estilo, corMsg, corVar)
#define debugCor(cor)

#endif //DEBUG_ON

//====================================================================================
//                   INICIALIZAÇÃO
//====================================================================================

void DEBUG_MODE();

#if DEBUG_ON && DEBUG_BOOT
    void DEBUG_SISTEMA_INTRO();
#endif //DEBUG_BOOT

#if DEBUG_ON && DEBUG_SYSTEM
    void debugCronometro(uint32_t ms);
#endif //DEBUG_SYSTEM