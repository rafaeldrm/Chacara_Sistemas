//====================================================================================
//                   DEPENDÊNCIAS
//====================================================================================

#include "Controle_Luz_Acesso_Reservatorio.h"
#include "DEBUG_MODE.h"

#include "Modulo_Reles.h"

#include "Tempo.h"
#include "Cronometro.h"
#include "Estados_Sistema_Eco_Hidro.h"

//====================================================================================
//                   VARIÁVEIS INTERNAS
//====================================================================================

static constexpr uint32_t TEMPO_LUZ_LIGADA = TEMPO::MINUTOS(15);

static CRONOMETRO::Temporizador tempoLuzAcessoReservatorio;

//====================================================================================
//                   SETUP
//====================================================================================

void Controle_Luz_Acesso_Reservatorio_Inicializa()
{

    Controle_Luz_Acesso_Reservatorio_Temporizado();
    tempoLuzAcessoReservatorio.Cronometro_Iniciar();

}

//====================================================================================
//                   LUZ DESLIGANDO
//====================================================================================

void Controle_Luz_Acesso_Reservatorio_Desligando()
{

    RELE_LUZ_ACESSO_RESERVATORIO.Desligar();
    LUZ_ACESSO::definir(LUZ_ACESSO::SITUACAO::DESLIGADO);

}

//====================================================================================
//                   LUZ COM TEMPORIZAÇÃO AUTOMÁTICA
//====================================================================================

void Controle_Luz_Acesso_Reservatorio_Temporizado()
{       

    RELE_LUZ_ACESSO_RESERVATORIO.Ligar();

    if (tempoLuzAcessoReservatorio.Cronometro_Dispara_Uma_Vez(TEMPO_LUZ_LIGADA))
    {
        tempoLuzAcessoReservatorio.Cronometro_Iniciar();
        Controle_Luz_Acesso_Reservatorio_Desligando();

    }

}

//====================================================================================
//                   LUZ FORÇADA (ORDEM KAMYLLA)
//====================================================================================

void Controle_Luz_Acesso_Reservatorio_Forcado()
{

    RELE_LUZ_ACESSO_RESERVATORIO.Ligar();

}

//====================================================================================
//                   LUZ SENDO CONSUMINDO (BATERIA)
//====================================================================================

void Controle_Luz_Acesso_Reservatorio_Consumindo()
{

    RELE_LUZ_ACESSO_RESERVATORIO.Ligar();

}