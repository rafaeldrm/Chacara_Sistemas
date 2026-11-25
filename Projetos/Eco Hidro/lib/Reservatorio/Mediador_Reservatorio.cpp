//====================================================================================
//                   DEPENDÊNCIAS
//====================================================================================

#include "Mediador_Reservatorio.h"
#include "DEBUG_MODE.h"

#include "Estados_Sistema_Eco_Hidro.h"

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

    if (sistemaModo == SISTEMA_ECO_HIDRO::ESTADO::BLOQUEIO
    || potencia == POTENCIA::ESTADO::PORTAO)
    {
        return; //Sistema Blqueado, ou Portão sendo Utilizado: não aciona o restante
    }

    //========= CASA TORRE FINALIZADO, VOLTAGEM CHEIA ================

    if (casaTorre == CASA_TORRE::ESTADO::LIBERADO_PARA_O_RESERVATORIO //requisito
        && bateria == VOLTAGEM::ESTADO::CHEIA //requisito
        && reservatorio == RESERVATORIO::ESTADO::NORMAL //trava
        && potencia == POTENCIA::ESTADO::REPOUSO) //trava
    {
        RESERVATORIO::definir(RESERVATORIO::ESTADO::LIBERADO);
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

    if (casaTorre == CASA_TORRE::ESTADO::NORMAL
        && (reservatorio == RESERVATORIO::ESTADO::FINALIZADO
        || reservatorio == RESERVATORIO::ESTADO::SO_MAIS_UM_POUQUINHO))
    {
        RESERVATORIO::definir(RESERVATORIO::ESTADO::NORMAL);
    }
}

//====================================================================================
//                   LOOP
//====================================================================================

void Mediador_Reservatorio_Atualiza()
{
    Mediador_Reservatorio_Prepara_Transferencia_Agua();

    Mediador_Reservatorio_Intervencao_Casa_Torre();
}