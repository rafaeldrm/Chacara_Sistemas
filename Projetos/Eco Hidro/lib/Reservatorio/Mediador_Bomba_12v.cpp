//====================================================================================
//                   DEPENDÊNCIAS
//====================================================================================

#include "Mediador_Bomba_12v.h"
#include "DEBUG_MODE.h"

#include "Estados_Sistema_Eco_Hidro.h"

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

    if (bomba12v == BOMBA_12V::ESTADO::REPOUSO
    && reservatorio == RESERVATORIO::ESTADO::LIBERADO) 
    {
        BOMBA_12V::definir(BOMBA_12V::ESTADO::LIBERADO);
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

    
    if (potencia == POTENCIA::ESTADO::BOMBA_12V
        && unidadePotencia == UNIDADE_POTENCIA::ESTADO::LIGADO
        && bombaReservatorio == BOMBA_12V::ESTADO::AGUARDA_LIBERACAO)
    {
        BOMBA_12V::definir(BOMBA_12V::ESTADO::LIGANDO_BOMBA);
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

    if (bomba12V == BOMBA_12V::ESTADO::BOMBA_LIGADA
    && bombaCasa == BOMBA_CASA::ESTADO::AGUARDA_LIBERACAO)
    {
        BOMBA_12V::definir(BOMBA_12V::ESTADO::DESLIGANDO_BOMBA);
    }

    //=============== Término do Bombeamento normal ===========

    if (reservatorio != RESERVATORIO::ESTADO::FINALIZADO)
        return;

        
    // Só manda desligar uma vez: se já está parada ou desligando, não mexe.
    if (bomba12V != BOMBA_12V::ESTADO::REPOUSO
    &&  bomba12V != BOMBA_12V::ESTADO::DESLIGANDO_BOMBA)
    {
        BOMBA_12V::definir(BOMBA_12V::ESTADO::DESLIGANDO_BOMBA);
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
