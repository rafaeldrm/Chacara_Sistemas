//=============================== MEMORIA_RAM.h ===============================
#pragma once
#include <Arduino.h>


//=============================== SÍMBOLOS DO LINKER ===============================
// ATENÇÃO: ficam FORA do namespace, senão o linker procura MemoriaRam::__heap_start e dá erro

#if defined(__AVR__)
  extern char  __heap_start;
  extern char* __brkval;
#elif defined(__SAM3X8E__)
  extern "C" char* sbrk(int incr);
#else
  #error "Plataforma não suportada"
#endif


namespace Memoria_Ram {

//=============================== CONFIGURAÇÃO ===============================

#if defined(__AVR__)
  static constexpr uint32_t TOTAL_BYTES = 8192UL;
#else
  static constexpr uint32_t TOTAL_BYTES = 98304UL;
#endif

static constexpr uint8_t  PADRAO_PINTURA = 0xAA;
static constexpr uint16_t MARGEM_PILHA   = 64;     // folga pra função de pintura não se pintar


//=============================== ESTADO INTERNO ===============================

static char* inicioPintura = nullptr;



//=============================== FIM DO HEAP ===============================

inline char* fimHeap() {
#if defined(__AVR__)
  return __brkval ? __brkval : &__heap_start;
#else
  return sbrk(0);
#endif
}



//=============================== RAM LIVRE (INSTANTÂNEA) ===============================

inline uint32_t livre() {
  char topoPilha;                       // endereço dele ≈ ponteiro da stack agora
  return (uint32_t)(&topoPilha - fimHeap());
}



//=============================== RAM USADA (%) ===============================

inline uint8_t usadaPercentual() {
  return (uint8_t)(100UL - (livre() * 100UL) / TOTAL_BYTES);
}



//=============================== PINTURA (CHAMAR NO COMEÇO DO setup) ===============================

inline void pintar() {
  char marcador;
  char* fim = &marcador - MARGEM_PILHA;
  inicioPintura = fimHeap();

  for (char* p = inicioPintura; p < fim; p++) {
    *p = PADRAO_PINTURA;
  }
}

//=============================== MÍNIMO LIVRE JÁ OCORRIDO ===============================

inline uint32_t minimoLivre() {
  if (inicioPintura == nullptr) return 0;

  char* base = fimHeap();                       // onde o heap está AGORA
  if (base < inicioPintura) base = inicioPintura;

  char* p = base;
  while (*p == (char)PADRAO_PINTURA) p++;

  return (uint32_t)(p - base);
}
}  // namespace Memoria_Ram

void Monitora_RAM_Atualiza();