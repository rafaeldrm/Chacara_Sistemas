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

    if (casaTorre == CASA_TORRE::ETAPAS::FINALIZADO
    && reservatorio == RESERVATORIO::TRANSFERENCIA::NORMAL
    && bomba12V == BOMBA_12V::ESTAGIOS::REPOUSO //trava segurança evita acesso repentino nesse if.
    && potencia == POTENCIA::SELECIONADO::REPOUSO
    && bateria == VOLTAGEM::LEITURA::CHEIA
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
    if (RESERVATORIO::atual() != RESERVATORIO::TRANSFERENCIA::FINALIZADO)
        return;

    const auto bomba12V = BOMBA_12V::atual();

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
    Mediador_Reservatorio_Transferencia_Entre_Reservatorios();

    Mediador_Reservatorio_Transferencia_Finalizada();

    Mediador_Reservatorio_Libera_Para_Novo_Ciclo();   // sempre depois do desliga
}