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

//====================================================================================
//                   SETUP
//====================================================================================

void Mediador_Reservatorio_Inicializa()
{
        debugSetupDivisao(COR::AMARELO, "MEDIADOR RESERVATORIO INICIALIZANDO", COR::MAGENTA, "=", 0);
}

//====================================================================================
//                   ORDEM DRIMEL SYSTEM - TRANSFERÊNCIA DE ÁGUA ENTRE RESERVATÓRIOS
//====================================================================================

static void Mediador_Reservatorio_Ordem_Transferencia_Reservatorios()
{

    const auto ordemReservatorio = ORDENS_DRIMEL_SYSTEM::RESERVATORIO::atual();

    if (ordemReservatorio == ORDENS_DRIMEL_SYSTEM::RESERVATORIO::ORDEM::TRANSFERENCIA_ENTRE_RESERVATORIOS)
    {
        RESERVATORIO::definir(RESERVATORIO::TRANSFERENCIA::LIBERADO);
        BOMBA_12V::definir(BOMBA_12V::ESTAGIOS::LIBERADO);
        ORDENS_DRIMEL_SYSTEM::RESERVATORIO::definir(ORDENS_DRIMEL_SYSTEM::RESERVATORIO::ORDEM::NENHUM);
    }
}

//====================================================================================
//                   TRANSFERÊNCIA DE ÁGUA ENTRE OS RESERVATÓRIO 1 e o 2
//====================================================================================

static void Mediador_Reservatorio_Prepara_Transferencia_Agua()
{
    const auto casaTorre = CASA_TORRE::atual();
    const auto bateria = VOLTAGEM::atual();
    const auto reservatorio = RESERVATORIO::atual();
    const auto bomba12V = BOMBA_12V::atual();
    const auto sistemaModo = SISTEMA_ECO_HIDRO::atual();
    const auto potencia = POTENCIA::atual();

// ============ SISTEMA BLOQUEADO ===================================

    if (sistemaModo == SISTEMA_ECO_HIDRO::MODO::BLOQUEIO)
    {
        return;
    }

    //========= CASA TORRE FINALIZADO, VOLTAGEM CHEIA ================

    if (casaTorre == CASA_TORRE::ETAPAS::LIBERADO_PARA_O_RESERVATORIO
    && reservatorio == RESERVATORIO::TRANSFERENCIA::NORMAL
    && potencia == POTENCIA::SELECIONADO::REPOUSO
    && bateria == VOLTAGEM::LEITURA::CHEIA) 
    {
        RESERVATORIO::definir(RESERVATORIO::TRANSFERENCIA::LIBERADO);
        BOMBA_12V::definir(BOMBA_12V::ESTAGIOS::LIBERADO);
    }
}

//====================================================================================
//                   COORDENA DESLIGAMENTO DA BOMBA DO RESERVATÓRIO
//====================================================================================

static void Mediador_Reservatorio_Transferencia_Finalizada()
{
    const auto reservatorio = RESERVATORIO::atual();
    const auto bomba12V = BOMBA_12V::atual();

    if (reservatorio != RESERVATORIO::TRANSFERENCIA::FINALIZADO)
        return;

        
    // Só manda desligar uma vez: se já está parada ou desligando, não mexe.
    if (bomba12V != BOMBA_12V::ESTAGIOS::REPOUSO
    &&  bomba12V != BOMBA_12V::ESTAGIOS::DESLIGANDO_BOMBA)
    {
        BOMBA_12V::definir(BOMBA_12V::ESTAGIOS::DESLIGANDO_BOMBA);
    }
}

//====================================================================================
//                   RESERVATÓRIO ESQUECE O FINALIZADO QUANDO A CASA TORRE RECOMEÇA
//====================================================================================

static void Mediador_Reservatorio_Libera_Para_Novo_Ciclo()
{
    if (RESERVATORIO::atual() == RESERVATORIO::TRANSFERENCIA::FINALIZADO
    &&  CASA_TORRE::atual() == CASA_TORRE::ETAPAS::FINALIZADO)
    {
        RESERVATORIO::definir(RESERVATORIO::TRANSFERENCIA::NORMAL);
    }
}

//====================================================================================
//                   LOOP
//====================================================================================

void Mediador_Reservatorio_Atualiza()
{
    Mediador_Reservatorio_Ordem_Transferencia_Reservatorios();

    Mediador_Reservatorio_Prepara_Transferencia_Agua();

    Mediador_Reservatorio_Transferencia_Finalizada();

    Mediador_Reservatorio_Libera_Para_Novo_Ciclo();   // sempre depois do desliga
}