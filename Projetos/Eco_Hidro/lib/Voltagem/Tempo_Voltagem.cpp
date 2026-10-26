//====================================================================================
//                   DEPENDÊNCIAS
//====================================================================================

#include "Tempo_Voltagem.h"
#include "DEBUG_MODE.h"

#include "Tempo.h"
#include "Cronometro.h"

//====================================================================================
//                   VARIÁVEIS INTERNAS
//====================================================================================

static constexpr uint32_t TEMPO_CONFIRMACAO = TEMPO::MINUTOS(1);

static CRONOMETRO::Temporizador tempoVoltagem;

//====================================================================================
//                   SETUP
//====================================================================================

void Tempo_Voltagem_Inicializa()
{
    debugSetupDivisao(COR::AMARELO, "TEMPO VOLTAGEM INICIALIZANDO", COR::MAGENTA, "=", 45);

    tempoVoltagem.Cronometro_Iniciar();
}

//====================================================================================
//                  PROCESSA TEMPO DE VERIFICAÇÃO DA VOLTAGEM
//====================================================================================

bool Tempo_Voltagem_Processa()
{
    return tempoVoltagem.Cronometro_Ciclo_Continuo(TEMPO_CONFIRMACAO);
}