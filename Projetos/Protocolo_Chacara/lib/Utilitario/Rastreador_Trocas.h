//====================================================================================
//                   RASTREADOR DE TROCAS DE ESTADO (por loop)
//====================================================================================
//
//  OBJETIVO:
//  ---------
//  Detectar quando UMA MESMA variável de estado recebe DOIS OU MAIS valores
//  dentro de um ÚNICO loop() — essa é a assinatura exata de FSMs "se atropelando".
//  Também mostra o "atraso de 1 tick" causado pela ordem de execução dos mediadores.
//
//  COMO FUNCIONA (3 peças, zero overhead quando desligado):
//  ---------------------------------------------------------
//  1) TICK     : contador incrementado UMA vez por loop() (no início do loop).
//  2) ORIGEM   : ponteiro global que cada Gerencia/Mediador/Controle seta na
//                entrada da sua função _Atualiza/_Loop. O definir() lê ele.
//  3) DEFINIR  : no gerador (template), dentro de cada ::definir(), após o
//                "if (estado.x == novo) return;", adicionar 3 linhas que imprimem:
//                    [tick=1234] VARIAVEL : ANTIGO -> NOVO   @ Origem
//                Se a mesma variável aparecer duas vezes com o MESMO tick na
//                serial, você encontrou o atropelamento.
//
//  ATIVAÇÃO:
//  ---------
//  #define RASTREADOR_TROCAS_ON 1   (0 remove tudo do binário)
//
//  WIRING MÍNIMO (3 lugares):
//  --------------------------
//  A) No início de loop() (main.cpp):  TICK_LOOP::incrementar();
//  B) Na entrada de cada _Atualiza/_Loop:  //ORIGEM_FSM::setar(F("Mediador_Casa_Torre"));
//  C) No template do gerador (dentro de ::definir), logo depois do guarda igual:
//         RASTREAR_TROCA("BOMBA_CASA", texto(estado.bombaCasa), texto(novo));
//
//====================================================================================

#pragma once
#include <Arduino.h>
#include "DEBUG_MODE.h"

// 1 = logs ativos | 0 = compilado fora, não gasta 1 byte
#ifndef RASTREADOR_TROCAS_ON
#define RASTREADOR_TROCAS_ON 1
#endif

#if RASTREADOR_TROCAS_ON

namespace TICK_LOOP
{
    inline uint32_t& valor()
    {
        static uint32_t tick = 0;
        return tick;
    }
    inline void incrementar() { valor()++; }
    inline uint32_t atual()    { return valor(); }
}

namespace ORIGEM_FSM
{
    inline const char*& ptr()
    {
        static const char* origem = "?";
        return origem;
    }
    inline void setar(const __FlashStringHelper* o) { ptr() = reinterpret_cast<const char*>(o); }
    inline const char* atual()                       { return ptr(); }
}

// Impressão única (FlashStringHelper seguro para AVR)
inline void RastrearTroca_Imprimir(const char* var,
                                   const __FlashStringHelper* antigo,
                                   const __FlashStringHelper* novo)
{
    Serial.print(F("[tick="));
    Serial.print(TICK_LOOP::atual());
    Serial.print(F("] "));
    Serial.print(var);
    Serial.print(F(" : "));
    Serial.print(antigo);
    Serial.print(F(" -> "));
    Serial.print(novo);
    Serial.print(F("   @ "));
    Serial.println(reinterpret_cast<const __FlashStringHelper*>(ORIGEM_FSM::atual()));
}

// Macro para colocar DENTRO de cada ::definir() (no template do gerador).
// "var" é string literal (fica em RAM mas é 1 por enum, aceitável).
// "antigo" e "novo" são o retorno de texto() em __FlashStringHelper*.
#define RASTREAR_TROCA(var, antigo, novo) \
    RastrearTroca_Imprimir(var, antigo, novo)

#else

#define RASTREAR_TROCA(var, antigo, novo)

namespace TICK_LOOP  { inline void incrementar() {} inline uint32_t atual() { return 0; } }
namespace ORIGEM_FSM { inline void setar(const __FlashStringHelper*) {} inline const char* atual() { return "?"; } }

#endif // RASTREADOR_TROCAS_ON

//====================================================================================
//  EXEMPLO DE PATCH NO GERADOR (template do ::definir)
//====================================================================================
//
//  namespace BOMBA_CASA {
//      void definir(ESTAGIOS novo) {
//          if (estado.bombaCasa == novo) return;
//  +++     RASTREAR_TROCA("BOMBA_CASA", texto(estado.bombaCasa), texto(novo));
//          estado.bombaCasa = novo;
//          debugVariavel("ESTAGIOS bombaCasa : ", texto(novo), NENHUM, VERDE, AMARELO);
//      }
//  }
//
//  COMO LER A SAÍDA PARA ACHAR O ATROPELAMENTO:
//  --------------------------------------------
//  [tick=4521] BOMBA_CASA : AGUARDA_LIBERACAO -> LIBERADO   @ Mediador_Casa_Torre
//  [tick=4521] BOMBA_CASA : LIBERADO -> BLOQUEIO            @ Mediador_Casa_Torre
//   ^ mesmo tick, mesma variável, dois escritores = atropelamento confirmado.
//
//  [tick=4522] POTENCIA : REPOUSO -> BOMBA_CASA   @ Mediador_Potencia
//  [tick=4523] BOMBA_CASA : AGUARDA_LIBERACAO -> LIGANDO_BOMBA  @ Mediador_Casa_Torre
//   ^ POTENCIA só é vista por Liga_Bomba no tick seguinte = atraso de ordem.
//====================================================================================
