//====================================================================================
//                   DEPENDÊNCIAS
//====================================================================================

#include "Controle_Giro_Preventivo_Bomba_Casa.h"
#include "DEBUG_MODE.h"

#include "Tempo.h"
#include "Cronometro.h"

#include "Estados_Sistema_Eco_Hidro.h"

//====================================================================================
//                   VARIÁVEIS INTERNAS
//====================================================================================

static constexpr uint32_t TEMPO_COTIDIANO = TEMPO::MINUTOS(2); //1 dia (24 horas)

static constexpr uint32_t TEMPO_EXERCICIO = TEMPO::SEGUNDOS(5);

static CRONOMETRO::Temporizador contadorCotidiano;

static CRONOMETRO::Temporizador bombeamentoExercicio;

//====================================================================================
//                   SETUP
//====================================================================================

void Controle_Giro_Preventivo_Bomba_Casa_Inicializa()
{
        debugSetupDivisao(COR::AMARELO, "CONTROLE GIRO PREVENTIVO BOMBA CASA INICIALIZANDO", COR::MAGENTA, "=", 0);
        GIRO_PREVENTIVO_BOMBA_CASA::definir(GIRO_PREVENTIVO_BOMBA_CASA::ESTADO::INICIA_CONTAGEM_COTIDIANO);
}

//====================================================================================
//                   INICIA CONTAGEM COTIDIANO - GIRO PREVENTIVO BOMBA CASA
//====================================================================================

void Controle_Giro_Preventivo_Bomba_Casa_Inicia_Contagem_Cotidiano()
{
        contadorCotidiano.Cronometro_Iniciar();
        GIRO_PREVENTIVO_BOMBA_CASA::definir(GIRO_PREVENTIVO_BOMBA_CASA::ESTADO::VERIFICA_CONTAGEM_COTIDIANO);
}

//====================================================================================
//                   VERIFICA CONTAGEM COTIDIANO
//====================================================================================

void Controle_Giro_Preventivo_Bomba_Casa_Verifica_Contagem_Cotidiano()
{
        if (contadorCotidiano.Cronometro_Dispara_Uma_Vez(TEMPO_COTIDIANO))
        {
                contadorCotidiano.Cronometro_Cancelar();
                GIRO_PREVENTIVO_BOMBA_CASA::definir(GIRO_PREVENTIVO_BOMBA_CASA::ESTADO::PREPARA_EXERCICIO_BOMBA);
        }
}

//====================================================================================
//                   CANCELA CONTAGEM COTIDIANO
//====================================================================================

void Controle_Giro_Preventivo_Bomba_Casa_Cancela_Contagem_Cotidiano()
{
        contadorCotidiano.Cronometro_Cancelar();
        GIRO_PREVENTIVO_BOMBA_CASA::definir(GIRO_PREVENTIVO_BOMBA_CASA::ESTADO::FINALIZADO);
}

//====================================================================================
//                   PREPARA EXERCÍCIO BOMBA
//====================================================================================


void Controle_Giro_Preventivo_Bomba_Casa_Prepara_Exercicio_Bomba()
{
        bombeamentoExercicio.Cronometro_Iniciar();
}

//====================================================================================
//                   INICIO EXERCÍCIO BOMBA
//====================================================================================

void Controle_Giro_Preventivo_Bomba_Casa_Inicio_Exercicio_Bomba()
{
        if (bombeamentoExercicio.Cronometro_Dispara_Uma_Vez(TEMPO_EXERCICIO))
        {
                GIRO_PREVENTIVO_BOMBA_CASA::definir(GIRO_PREVENTIVO_BOMBA_CASA::ESTADO::FIM_EXERCICIO_BOMBA);

        }
}

//====================================================================================
//                   FIM EXERCÍCIO BOMBA
//====================================================================================

void Controle_Giro_Preventivo_Bomba_Casa_Fim_Exercicio_Bomba()
{
        contadorCotidiano.Cronometro_Cancelar();
        bombeamentoExercicio.Cronometro_Cancelar();
        GIRO_PREVENTIVO_BOMBA_CASA::definir(GIRO_PREVENTIVO_BOMBA_CASA::ESTADO::FINALIZADO);
}