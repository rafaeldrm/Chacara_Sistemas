//====================================================================================
//                   DEPENDÊNCIAS
//====================================================================================

#include "Controle_Anti_Transbordamento_Caixa_Torre.h"
#include "DEBUG_MODE.h"

#include "Tempo.h"
#include "Cronometro.h"

#include "Estados_Sistema_Eco_Hidro.h"

//====================================================================================
//                   VARIÁVEIS INTERNAS
//====================================================================================

static constexpr uint32_t TEMPO_MAXIMO_BOMBEAMENTO = TEMPO::MINUTOS(25); //27UL * 60UL * 1000UL; // 27 min
 
static CRONOMETRO::Temporizador antiTransbordamento;

static bool aguardandoInicioContagem = false;


//====================================================================================
//                   SETUP
//====================================================================================

void Controle_Anti_Transbordamento_Caixa_Torre_Inicializa()
{
        debugSetupDivisao(COR::AMARELO, "CONTROLE ANTI TRANSBORDAMENTO CAIXA TORRE INICIALIZANDO", COR::MAGENTA, "=", 0);
}

//====================================================================================
//                   INICIA CONTAGEM CAIXA TORRE PROTEÇÃO ANTI-TRANSBORDAMENTO
//====================================================================================

static void Controle_Anti_Transbordamento_Caixa_Torre_Inicia_Contagem()
{
    antiTransbordamento.Cronometro_Iniciar();
}

//====================================================================================
//                   PREPARA ACIONAMENTO DE TRAVA ANTI TRANSBORDAMENTO
//====================================================================================

static void Controle_Anti_Transbordamento_Caixa_Torre_Prepara_Acionamento()
{
    const auto bombaCasa = BOMBA_CASA::atual();
    const auto casaTorre = CASA_TORRE::atual();

    if (bombaCasa == BOMBA_CASA::ESTADO::AGUARDA_LIBERACAO
        && (casaTorre == CASA_TORRE::ESTADO::PRIMEIRO_CICLO_ENCHIMENTO_CAIXA
            || casaTorre == CASA_TORRE::ESTADO::SEGUNDO_CICLO_ENCHIMENTO_CAIXA))
        {
            aguardandoInicioContagem = true;
        }

    //=========================================================

    if (bombaCasa == BOMBA_CASA::ESTADO::BOMBA_LIGADA
        && aguardandoInicioContagem == true)
    {
    Controle_Anti_Transbordamento_Caixa_Torre_Inicia_Contagem();
    aguardandoInicioContagem = false;
    }

}

//====================================================================================
//                   CANCELAMENTO CONTADOR ANTI TRANSBORDAMENTO
//====================================================================================

static void Controle_Anti_Transbordamento_Caixa_Torre_Cancelamento()
{
    const auto bombaCasa = BOMBA_CASA::atual();

    if (bombaCasa == BOMBA_CASA::ESTADO::DESLIGANDO_BOMBA)
    {
        antiTransbordamento.Cronometro_Cancelar();
        aguardandoInicioContagem = false;
    }

}

//====================================================================================
//                   LOOP
//====================================================================================

void Controle_Anti_Transbordamento_Caixa_Torre_Atualiza()
{
    Controle_Anti_Transbordamento_Caixa_Torre_Prepara_Acionamento();

    if (antiTransbordamento.Cronometro_Dispara_Uma_Vez(TEMPO_MAXIMO_BOMBEAMENTO))
    {
        DESLIGAMENTO_BOMBA::definir(DESLIGAMENTO_BOMBA::ESTADO::TEMPO_MAXIMO);
        BOMBA_CASA::definir(BOMBA_CASA::ESTADO::DESLIGANDO_BOMBA);
    }

    Controle_Anti_Transbordamento_Caixa_Torre_Cancelamento();
}
