//====================================================================================
//                   DEPENDÊNCIAS
//====================================================================================

#include "Monitor_Latencia.h"
#include "DEBUG_MODE.h"

#include <stdint.h>
#include "Tempo.h"

//====================================================================================
//                   VARIÁVEIS INTERNAS
//====================================================================================

namespace
{
    //============================================================
    // CONFIGURAÇÕES
    //============================================================

    static constexpr uint32_t INTERVALO_IMPRESSAO_5_SEGUNDOS = TEMPO::SEGUNDOS(5);
    static constexpr uint32_t INTERVALO_DIARIO  = TEMPO::DIAS(1);

    //============================================================
    // CONTROLE DE TEMPO
    //============================================================

    uint32_t inicioLoopMicrosSegundo      = 0;
    uint32_t inicioPeriodoMilisSegundo    = 0;
    uint32_t ultimoPrintMilisSegundo     = 0;

    //============================================================
    // ESTATÍSTICAS
    //============================================================

    uint32_t minimoMicrosSegundo = 0;
    uint32_t maximoMicrosSegundo = 0;

    uint64_t somaMicrosSegundo       = 0;
    uint64_t quantidade  = 0;

    bool iniciado             = false;
    bool extremosInicializados = false;
}

//============================================================
// INICIALIZAÇÃO
//============================================================

void Monitor_Latencia::Iniciar()
{
    inicioLoopMicrosSegundo    = micros();
    inicioPeriodoMilisSegundo = millis();
    ultimoPrintMilisSegundo   = inicioPeriodoMilisSegundo;

    minimoMicrosSegundo = 0;
    maximoMicrosSegundo = 0;

    somaMicrosSegundo      = 0;
    quantidade = 0;

    extremosInicializados = false;
    iniciado = true;
}

//============================================================
// FINALIZAÇÃO DO CICLO
//============================================================

void Monitor_Latencia::Finalizar()
{
    if (!iniciado)
        return;

    const uint32_t agoraMicrosSegundo = micros();
    const uint32_t agoraMilisSegundo = millis();

    const uint32_t latenciaMicrosSegundo = agoraMicrosSegundo - inicioLoopMicrosSegundo;

    //============================================================
    // REINICIALIZA MIN/MAX A CADA 24 HORAS
    //============================================================

    if ((uint32_t)(agoraMilisSegundo - inicioPeriodoMilisSegundo) >= INTERVALO_DIARIO)
    {
        extremosInicializados = false;
        inicioPeriodoMilisSegundo = agoraMilisSegundo;
    }

    //============================================================
    // ATUALIZA MIN/MAX
    //============================================================

    if (!extremosInicializados)
    {
        minimoMicrosSegundo = latenciaMicrosSegundo;
        maximoMicrosSegundo = latenciaMicrosSegundo;

        extremosInicializados = true;
    }
    else
    {
        if (latenciaMicrosSegundo < minimoMicrosSegundo)
            minimoMicrosSegundo = latenciaMicrosSegundo;

        if (latenciaMicrosSegundo > maximoMicrosSegundo)
            maximoMicrosSegundo = latenciaMicrosSegundo;
    }

    //============================================================
    // ATUALIZA MÉDIA ACUMULADA
    //============================================================

    somaMicrosSegundo += latenciaMicrosSegundo;
    quantidade++;

    //============================================================
    // IMPRESSÃO PERIÓDICA
    //============================================================

    if ((uint32_t)(agoraMilisSegundo - ultimoPrintMilisSegundo) >= INTERVALO_IMPRESSAO_5_SEGUNDOS)
    {
        const uint32_t mediaMicrosSegundo = (uint32_t)(somaMicrosSegundo / quantidade);

        Serial.print(F("Volta Rápida= "));
        Serial.print(minimoMicrosSegundo);
        Serial.print(F(" /  Média Volta= "));
        Serial.print(mediaMicrosSegundo);
        Serial.print(F(" / Volta Lenta= "));
        Serial.print(maximoMicrosSegundo);
        Serial.println(F(" Micros Segundo"));

        ultimoPrintMilisSegundo = agoraMilisSegundo;
    }

    //============================================================
    // REFERÊNCIA PARA O PRÓXIMO CICLO
    //============================================================

    inicioLoopMicrosSegundo = micros();
}
