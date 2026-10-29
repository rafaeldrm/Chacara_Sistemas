//====================================================================================
//                   DEPENDÊNCIAS
//====================================================================================

#include "DEBUG_MODE.h"

/*
OBSERVAÇÃO ============================================================
.Por ser Arquivo Universal logo é natural ter possíveis funções que um ou outro sistema utilize, e
outros sistema nem chegue a utilizar, como o modo RunTime que é preferência até o momento para o Sistema Reservatório
Pode Ser Adicionados mais outras formas de leitura e monitoramento dependendo de como o Sistema precise ser interpretado
*/

//====================================================================================
//                   VARIÁVEL/INTRODUÇÃO
//====================================================================================
#if DEBUG_ON && DEBUG_BOOT

static const char* DATA = __DATE__;  // Data da compilação
static const char* HORA = __TIME__;  // Hora da compilação

#endif  //DEBUG_BOOT
//====================================================================================
//                   INICIALIZAÇÃO
//====================================================================================

void DEBUG_MODE()
{
    #if DEBUG_ON
   #if DEBUG_BOOT

        debugSetupArquivo();

        DEBUG_SISTEMA_INTRO();

    #endif //DEBUG_BOOT

    #endif //DEBUG_ON

}

//====================================================================================
//                   CRONOMETRO PARA MILLIS CASO PRECISE MONITORAR
//====================================================================================
#if DEBUG_ON
#if DEBUG_SYSTEM

void debugCronometro(uint32_t ms)
{
    uint16_t totalSeg = ms / 1000;          // Converte milissegundos do millis() para segundos inteiros
    uint8_t segundos = totalSeg % 60;       // O resto da divisão por 60 isola os segundos do minuto atual
    uint8_t minutos = (totalSeg / 60) % 60; // Divide por 60 para minutos e isola o resto para ignorar as horas
    uint8_t horas = totalSeg / 3600;        // 3600 (60min * 60seg) extrai a quantidade total de horas decorridas
    Serial.print(F("["));
    if (horas < 10)
        Serial.print(F("0"));
    Serial.print(horas);
    Serial.print(F(":"));
    if (minutos < 10)
        Serial.print(F("0"));
    Serial.print(minutos);
    Serial.print(F(":"));
    if (segundos < 10)
        Serial.print(F("0"));
    Serial.print(segundos);
    Serial.print(F("] "));
}

#endif //DEBUG_SYSTEM

//====================================================================================
//                   INTRODUCÃO DE BOOT do SISTEMA
//====================================================================================
//BOOT nome Sistema Reservatório em ASCII
//SITE  para CRIAÇÃO ::  https://patorjk.com/software/taag
#if DEBUG_BOOT

void DEBUG_SISTEMA_INTRO(){  
    debugVariavel("COMPILADO EM : ", DATA, NEGRITO, VERDE, AMARELO);
    debugVariavel("ÀS : ", HORA, NEGRITO, VERDE, AMARELO);
}

#endif //DEBUG_BOOT  
#endif //DEBUG_ON