//====================================================================================
//                   DEPENDÊNCIAS
//====================================================================================

#include "Mediador_Reservatorio.h"
#include "DEBUG_MODE.h"

#include "Estados_Sistema_Eco_Hidro.h"
#include "Estados_Eventos_Eco_Hidro.h"

//====================================================================================
//                   VARIÁVEIS INTERNAS
//====================================================================================

// Enquanto o sistema de Voltagem estiver em off (bateria sempre lendo 0 V),
// exigir bateria CHEIA trava a transferência para sempre.
// Quando a Voltagem estiver madura: troque para true.
static constexpr bool EXIGE_BATERIA_CHEIA = false;

//====================================================================================
//                   SETUP
//====================================================================================

void Mediador_Reservatorio_Inicializa()
{
        debugSetupDivisao(COR::AMARELO, "MEDIADOR RESERVATORIO INICIALIZANDO", COR::MAGENTA, "=", 45);
}

//====================================================================================
//                   TRANSFERÊNCIA DE ÁGUA ENTRE OS RESERVATÓRIO 1 e o 2
//====================================================================================

static void Mediador_Reservatorio_Transferencia_Entre_Reservatorios()
{
    const auto casaTorre = CASA_TORRE::atual();
    const auto bateria = VOLTAGEM::atual();
    const auto reservatorio = RESERVATORIO::atual();
    const auto bomba12V = BOMBA_12V::atual();
    const auto sistemaModo = SISTEMA_ECO_HIDRO::atual();
    const auto potencia = POTENCIA::atual();

// -------------------

    const bool bateriaOk =
        !EXIGE_BATERIA_CHEIA || bateria == VOLTAGEM::LEITURA::CHEIA;

    const bool potenciaLivre =
        potencia == POTENCIA::SELECIONADO::REPOUSO; // espera a Casa_Torre terminar e o inversor apagar

// -------------------

    if (casaTorre == CASA_TORRE::ETAPAS::FINALIZADO
    && reservatorio == RESERVATORIO::TRANSFERENCIA::NORMAL
    && bomba12V == BOMBA_12V::ESTAGIOS::REPOUSO //trava segurança evita acesso repentino nesse if.
    && potenciaLivre
    && bateriaOk
    && sistemaModo != SISTEMA_ECO_HIDRO::MODO::BLOQUEIO)   //se for bloqueio do sistema logo nunca acessará essa parte do if.
    {
        RESERVATORIO::definir(RESERVATORIO::TRANSFERENCIA::LIBERADO);

        POTENCIA::definir(POTENCIA::SELECIONADO::BOMBA_12V);  //PEDE a potência (quem executa é o Mediador_Unidade_Potencia)
        BOMBA_12V::definir(BOMBA_12V::ESTAGIOS::LIBERADO);
    }
}

//====================================================================================
//                   COORDENA DESLIGAMENTO DA BOMBA DO RESERVATÓRIO
//====================================================================================

static void Mediador_Reservatorio_Transferencia_Finalizada()
{
    const auto reservatorio = RESERVATORIO::atual();

    if (reservatorio == RESERVATORIO::TRANSFERENCIA::FINALIZADO)
    {
        // Só desliga A BOMBA. O inversor e a fila (POTENCIA) são do Mediador_Unidade_Potencia.
        BOMBA_12V::definir(BOMBA_12V::ESTAGIOS::DESLIGANDO_BOMBA);
        RESERVATORIO::definir(RESERVATORIO::TRANSFERENCIA::NORMAL);
        CASA_TORRE::definir(CASA_TORRE::ETAPAS::NORMAL);
    }
}

//====================================================================================
//                   LOOP
//====================================================================================

void Mediador_Reservatorio_Atualiza()
{
    Mediador_Reservatorio_Transferencia_Entre_Reservatorios();

    Mediador_Reservatorio_Transferencia_Finalizada();
}