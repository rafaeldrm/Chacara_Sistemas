//====================================================================================
//                   DEPENDÊNCIAS
//====================================================================================

#include "Monitora_RAM.h"
#include "DEBUG_MODE.h"

//====================================================================================
//                   VARIÁVEIS INTERNAS
//====================================================================================

//====================================================================================
//                   LOOP
//====================================================================================

void Monitora_RAM_Atualiza()
{
    static uint32_t ultimoPrint = 0;

    if ((uint32_t)(millis() - ultimoPrint) < 1000)
        return;

    ultimoPrint = millis();

    Serial.print(F("Livre: "));
    Serial.print(Memoria_Ram::livre());

    Serial.print(F(" | Mínimo: "));
    Serial.print(Memoria_Ram::minimoLivre());

    Serial.print(F(" | Usada: "));
    Serial.print(Memoria_Ram::usadaPercentual());

    Serial.print(F(" Heap cresceu: "));
    Serial.println(
        (uint16_t)(Memoria_Ram::fimHeap() - Memoria_Ram::inicioPintura)
    );
}