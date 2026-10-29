//====================================================================================
//                   DEPENDÊNCIAS
//====================================================================================

#include "Mediador_Casa_Torre.h"
#include "DEBUG_MODE.h"

#include "Estados_Sistema_Eco_Hidro.h"
#include "Estados_Eventos_Eco_Hidro.h"

//====================================================================================
//                   VARIÁVEIS INTERNAS
//====================================================================================


//====================================================================================
//                   SETUP
//====================================================================================

void Mediador_Casa_Torre_Inicializa()
{
        debugSetupDivisao(COR::AMARELO, "MEDIADOR CASA TORRE INICIALIZANDO", COR::MAGENTA, "=", 45);
}

//====================================================================================
//                   COORDENA PRIMEIRO CICLO CASA TORRE BOMBA CASA
//====================================================================================

static void Mediador_Casa_Torre_Primeiro_Ciclo()
{
    const auto casaTorre = CASA_TORRE::atual();
    const auto bombaCasa = BOMBA_CASA::atual();
    const auto ordemCasaTorre = ORDENS_DRIMEL_SYSTEM::CASA_TORRE::atual();
    const auto sistemaModo = SISTEMA_ECO_HIDRO::atual();
    const auto potencia = POTENCIA::atual();

// -------------------

    const bool cicloInternoPronto =
        casaTorre == CASA_TORRE::ETAPAS::PRIMEIRO_CICLO_ENCHIMENTO_CAIXA
        && bombaCasa == BOMBA_CASA::ESTAGIOS::REPOUSO;

    const bool cicloExternoSolicitado =
        ordemCasaTorre == ORDENS_DRIMEL_SYSTEM::CASA_TORRE::ORDEM::CICLO_COMPLETO;

    const bool sistemaBloqueado =
        sistemaModo == SISTEMA_ECO_HIDRO::MODO::BLOQUEIO;

    const bool potenciaLivre =
        potencia == POTENCIA::SELECIONADO::REPOUSO;

// -------------------

    if (sistemaBloqueado)
        {
            return; //Sistema Bloqueado: não aciona a bomba d' água.
        }

    if (!potenciaLivre)
        {
            return; //Alguém está usando o inversor. Espera a vez (o pedido continua valendo).
        }

// -------------------

    if (cicloInternoPronto || cicloExternoSolicitado) //Condição ou Eco Hidro ou Drimel System(LoRa)
    {
        DESLIGAMENTO_BOMBA::definir(DESLIGAMENTO_BOMBA::MOTIVO::NENHUM); //resetando o registro anterior informado da proteção bomba.

        POTENCIA::definir(POTENCIA::SELECIONADO::BOMBA_CASA);            //PEDE a potência (quem executa é o Mediador_Unidade_Potencia)
        BOMBA_CASA::definir(BOMBA_CASA::ESTAGIOS::LIBERADO);

        ORDENS_DRIMEL_SYSTEM::CASA_TORRE::definir(ORDENS_DRIMEL_SYSTEM::CASA_TORRE::ORDEM::NENHUM); //reseta "mensagem pedido Drimel System via LoRa"
    }
}

//====================================================================================
//                   COORDENA SEGUNDO CICLO CASA TORRE BOMBA CASA
//====================================================================================

static void Mediador_Casa_Torre_Segundo_Ciclo()
{
    const auto casaTorre = CASA_TORRE::atual();
    const auto bombaCasa = BOMBA_CASA::atual();
    const auto ordemCasaTorre = ORDENS_DRIMEL_SYSTEM::CASA_TORRE::atual();
    const auto sistemaModo = SISTEMA_ECO_HIDRO::atual();
    const auto potencia = POTENCIA::atual();

// -------------------

    const bool sistemaBloqueado =
        sistemaModo == SISTEMA_ECO_HIDRO::MODO::BLOQUEIO;

    const bool potenciaLivre =
        potencia == POTENCIA::SELECIONADO::REPOUSO;

    const bool meioCicloInternoPronto =
        casaTorre == CASA_TORRE::ETAPAS::SEGUNDO_CICLO_ENCHIMENTO_CAIXA
        && bombaCasa == BOMBA_CASA::ESTAGIOS::REPOUSO;

    const bool meioCicloExternoSolicitado =
        ordemCasaTorre == ORDENS_DRIMEL_SYSTEM::CASA_TORRE::ORDEM::MEIO_CICLO;

// -------------------

    if (sistemaBloqueado)
        {
            return; //Sistema Bloqueado: não aciona a bomba d' água.
        }

    if (!potenciaLivre)
        {
            return; //Espera a vez.
        }

// -------------------

    if (meioCicloInternoPronto || meioCicloExternoSolicitado)
    {
        DESLIGAMENTO_BOMBA::definir(DESLIGAMENTO_BOMBA::MOTIVO::NENHUM); //resetando o registro anterior informado da proteção bomba.

        POTENCIA::definir(POTENCIA::SELECIONADO::BOMBA_CASA);            //PEDE a potência
        BOMBA_CASA::definir(BOMBA_CASA::ESTAGIOS::LIBERADO);

        ORDENS_DRIMEL_SYSTEM::CASA_TORRE::definir(ORDENS_DRIMEL_SYSTEM::CASA_TORRE::ORDEM::NENHUM);
    }
}

//====================================================================================
//                   COORDENA DESLIGAMENTO DA BOMBA DA CASA
//====================================================================================

static void Mediador_Casa_Torre_Desliga_Bomba()
{
    const auto bombaCasa = BOMBA_CASA::atual();
    const auto casaTorre = CASA_TORRE::atual();
    const auto ordemCasaTorre = ORDENS_DRIMEL_SYSTEM::CASA_TORRE::atual();

    if (bombaCasa != BOMBA_CASA::ESTAGIOS::BOMBA_LIGADA)
        return;

    if (casaTorre == CASA_TORRE::ETAPAS::AGUARDANDO_TRANSFERENCIA_GRAVIDADE
        || casaTorre == CASA_TORRE::ETAPAS::FINALIZADO
        || ordemCasaTorre == ORDENS_DRIMEL_SYSTEM::CASA_TORRE::ORDEM::DESLIGA)
    {
        // Só desliga A BOMBA. O inversor e a fila (POTENCIA) são do Mediador_Unidade_Potencia.
        BOMBA_CASA::definir(BOMBA_CASA::ESTAGIOS::DESLIGANDO_BOMBA);
        ORDENS_DRIMEL_SYSTEM::CASA_TORRE::definir(ORDENS_DRIMEL_SYSTEM::CASA_TORRE::ORDEM::NENHUM);
    }
}

//====================================================================================
//                   LOOP
//====================================================================================

void Mediador_Casa_Torre_Atualiza()
{
        Mediador_Casa_Torre_Primeiro_Ciclo();

        Mediador_Casa_Torre_Segundo_Ciclo();

        Mediador_Casa_Torre_Desliga_Bomba();
}