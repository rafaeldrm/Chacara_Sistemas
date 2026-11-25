//====================================================================================
//                   DEPENDÊNCIAS
//====================================================================================

#include "Controle_Luz_Acesso_Reservatorio.h"
#include "DEBUG_MODE.h"

#include "Atuadores/Modulo_Reles.h"

#include "Tempo.h"
#include "Cronometro.h"
#include "Estados_Sistema_Eco_Hidro.h"

//====================================================================================
//                   VARIÁVEIS INTERNAS
//====================================================================================

static constexpr uint32_t TEMPO_LUZ_LIGADA = TEMPO::SEGUNDOS(30);

static CRONOMETRO::Temporizador tempoLuzAcessoReservatorio;

//====================================================================================
//                   SETUP
//====================================================================================

void Controle_Luz_Acesso_Reservatorio_Inicializa()
{
        debugSetupDivisao(COR::AMARELO, "CONTROLE LUZ ACESSO RESERVATÓRIO INICIALIZANDO", COR::MAGENTA, "=", 0);
}

//====================================================================================
//                   INICIA TEMPORIZAÇÃO DA LUZ
//====================================================================================

void Controle_Luz_Acesso_Reservatorio_Iniciando_Temporizacao()
{

    tempoLuzAcessoReservatorio.Cronometro_Iniciar();

    LUZ_ACESSO::definir(LUZ_ACESSO::ESTADO::TEMPORIZADO);

}

//====================================================================================
//                   LUZ COM TEMPORIZAÇÃO AUTOMÁTICA
//====================================================================================

void Controle_Luz_Acesso_Reservatorio_Temporizado()
{       

    RELE_LUZ_ACESSO_RESERVATORIO.Ligar();

    if (tempoLuzAcessoReservatorio.Cronometro_Dispara_Uma_Vez(TEMPO_LUZ_LIGADA))
    {
        LUZ_ACESSO::definir(LUZ_ACESSO::ESTADO::DESLIGANDO);
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
//                   LUZ ACIONADO
//====================================================================================

void Controle_Luz_Acesso_Reservatorio_Acionado()
{

    RELE_LUZ_ACESSO_RESERVATORIO.Ligar();
}

//====================================================================================
//                   LUZ DESLIGANDO
//====================================================================================

void Controle_Luz_Acesso_Reservatorio_Desligando()
{

    RELE_LUZ_ACESSO_RESERVATORIO.Desligar();
    LUZ_ACESSO::definir(LUZ_ACESSO::ESTADO::DESLIGADO);

}

//====================================================================================
//                   LUZ SENDO CONSUMINDO (BATERIA)
//====================================================================================

void Controle_Luz_Acesso_Reservatorio_Consumindo()
{

    RELE_LUZ_ACESSO_RESERVATORIO.Ligar();

}