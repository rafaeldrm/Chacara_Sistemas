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

//@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@

//                      ORDEM DRIMEL

//@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@

//====================================================================================
//                   TRANSFERÊNCIA DE ÁGUA ENTRE RESERVATÓRIOS 1 E 2
//====================================================================================

static void Mediador_Reservatorio_Ordem_Transferencia()
{
    const auto ordemReservatorio = ORDENS_DRIMEL_SYSTEM::RESERVATORIO::atual();

    if (ordemReservatorio == ORDENS_DRIMEL_SYSTEM::RESERVATORIO::ORDEM::TRANSFERENCIA_ENTRE_RESERVATORIOS)
    {
        RESERVATORIO::definir(RESERVATORIO::TRANSFERENCIA::LIBERADO);
        BOMBA_12V::definir(BOMBA_12V::ESTAGIOS::LIBERADO);
        ORDENS_DRIMEL_SYSTEM::RESERVATORIO::definir(ORDENS_DRIMEL_SYSTEM::RESERVATORIO::ORDEM::NENHUM);
    }
}

//@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@

//                      RESERVATÓRIO

//@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@

//====================================================================================
//                   TRANSFERÊNCIA DE ÁGUA ENTRE OS RESERVATÓRIO 1 e o 2
//====================================================================================

static void Mediador_Reservatorio_Prepara_Transferencia_Agua()
{
    const auto casaTorre = CASA_TORRE::atual();
    const auto bateria = VOLTAGEM::atual();
    const auto reservatorio = RESERVATORIO::atual();
    const auto sistemaModo = SISTEMA_ECO_HIDRO::atual();
    const auto potencia = POTENCIA::atual();

// ============ SISTEMA BLOQUEADO ===================================

    if (sistemaModo == SISTEMA_ECO_HIDRO::MODO::BLOQUEIO
    || potencia == POTENCIA::SELECIONADO::PORTAO)
    {
        return; //Sistema Blqueado, ou Portão sendo Utilizado: não aciona o restante
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
//                   COORDENA ACIONAMENTO BOMBA RESERVATÓRIO
//====================================================================================

static void Mediador_Reservatorio_Aciona_Bomba_12V()
{
    const auto bombaReservatorio = BOMBA_12V::atual();
    const auto potencia = POTENCIA::atual();
    const auto unidadePotencia = UNIDADE_POTENCIA::atual();

    
    if (potencia == POTENCIA::SELECIONADO::BOMBA_12V
        && unidadePotencia == UNIDADE_POTENCIA::ESTAGIOS::LIGADO
        && bombaReservatorio == BOMBA_12V::ESTAGIOS::AGUARDA_LIBERACAO)
    {
        BOMBA_12V::definir(BOMBA_12V::ESTAGIOS::LIGANDO_BOMBA);
    }
}

//====================================================================================
//                   COORDENA DESLIGAMENTO DA BOMBA DO RESERVATÓRIO
//====================================================================================

static void Mediador_Reservatorio_Transferencia_Finalizada()
{
    const auto bombaCasa = BOMBA_CASA::atual();
    const auto bomba12V = BOMBA_12V::atual();
    const auto reservatorio = RESERVATORIO::atual();

    if (bomba12V == BOMBA_12V::ESTAGIOS::BOMBA_LIGADA
    && bombaCasa == BOMBA_CASA::ESTAGIOS::AGUARDA_LIBERACAO)
    {
        BOMBA_12V::definir(BOMBA_12V::ESTAGIOS::DESLIGANDO_BOMBA);
    }

    if (reservatorio != RESERVATORIO::TRANSFERENCIA::FINALIZADO)
        return;

        
    // Só manda desligar uma vez: se já está parada ou desligando, não mexe.
    if (bomba12V != BOMBA_12V::ESTAGIOS::REPOUSO
    &&  bomba12V != BOMBA_12V::ESTAGIOS::DESLIGANDO_BOMBA)
    {
        BOMBA_12V::definir(BOMBA_12V::ESTAGIOS::DESLIGANDO_BOMBA);
    }
}

//@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@

//                      RESERVATÓRIO - CASA TORRE

//@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@

//====================================================================================
//                   RESETANDO POSICIONAMENTO DO RESERVATÓRIO
//====================================================================================

static void Mediador_Reservatorio_Intervencao_Casa_Torre()
{
    const auto reservatorio = RESERVATORIO::atual();
    const auto casaTorre = CASA_TORRE::atual();

    if (casaTorre == CASA_TORRE::ETAPAS::NORMAL
        && (reservatorio == RESERVATORIO::TRANSFERENCIA::FINALIZADO
        || reservatorio == RESERVATORIO::TRANSFERENCIA::SO_MAIS_UM_POUQUINHO))
    {
        RESERVATORIO::definir(RESERVATORIO::TRANSFERENCIA::NORMAL);
    }
}

//====================================================================================
//                   LOOP
//====================================================================================

void Mediador_Reservatorio_Atualiza()
{
//    Mediador_Reservatorio_Ordem_Transferencia_Reservatorios();

    Mediador_Reservatorio_Prepara_Transferencia_Agua();

    Mediador_Reservatorio_Aciona_Bomba_12V();

    Mediador_Reservatorio_Transferencia_Finalizada();

    Mediador_Reservatorio_Intervencao_Casa_Torre();
}