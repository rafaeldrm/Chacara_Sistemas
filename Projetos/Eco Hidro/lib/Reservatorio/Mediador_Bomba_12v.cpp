//====================================================================================
//                   DEPENDÊNCIAS
//====================================================================================

#include "Mediador_Bomba_12v.h"
#include "DEBUG_MODE.h"

#include "FSM/Estados_Sistema_Eco_Hidro.h"

//====================================================================================
//                   VARIÁVEIS INTERNAS
//====================================================================================

//====================================================================================
//                   SETUP
//====================================================================================

void Mediador_Bomba_12v_Inicializa()
{
        debugSetupDivisao(COR::AMARELO, "MEDIADOR BOMBA 12V INICIALIZANDO", COR::MAGENTA, "=", 0);
}

//====================================================================================
//                   PREPARA ACIONAMENTO BOMBA RESERVATÓRIO
//====================================================================================

static void Mediador_Bomba_12v_Prepara_Acionamento()
{
    const auto bomba12v = BOMBA_12V::atual();
    const auto reservatorio = RESERVATORIO::atual();

    if (bomba12v == BOMBA_12V::ESTAGIOS::REPOUSO
    && reservatorio == RESERVATORIO::TRANSFERENCIA::LIBERADO) 
    {
        BOMBA_12V::definir(BOMBA_12V::ESTAGIOS::LIBERADO);
    }
}

//====================================================================================
//                   ACIONANDO BOMBA RESERVATÓRIO
//====================================================================================

static void Mediador_Bomba_12v_Ligando()
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
//                   DESLIGAMENTO BOMBA RESERVATÓRIO
//====================================================================================

static void Mediador_Bomba_12v_Desligamento()
{
    const auto bombaCasa = BOMBA_CASA::atual();
    const auto bomba12V = BOMBA_12V::atual();
    const auto reservatorio = RESERVATORIO::atual();

    //=============== Interrupção Casa Torre ===============

    if (bomba12V == BOMBA_12V::ESTAGIOS::BOMBA_LIGADA
    && bombaCasa == BOMBA_CASA::ESTAGIOS::AGUARDA_LIBERACAO)
    {
        BOMBA_12V::definir(BOMBA_12V::ESTAGIOS::DESLIGANDO_BOMBA);
    }

    //=============== Término do Bombeamento normal ===========

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
//                   LOOP
//====================================================================================

void Mediador_Bomba_12v_Atualiza()
{
    Mediador_Bomba_12v_Prepara_Acionamento();

    Mediador_Bomba_12v_Ligando();

    Mediador_Bomba_12v_Desligamento();

}
