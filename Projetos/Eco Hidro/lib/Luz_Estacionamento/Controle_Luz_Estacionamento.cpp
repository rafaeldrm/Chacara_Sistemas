//====================================================================================
//                   DEPENDÊNCIAS
//====================================================================================

#include "Controle_Luz_Estacionamento.h"
#include "DEBUG_MODE.h"

#include "Atuadores/Leitura_Botao.h"
#include "Atuadores/Modulo_Reles.h"

#include "Tempo.h"
#include "Cronometro.h"
#include "Estados_Sistema_Eco_Hidro.h"

//====================================================================================
//                   VARIÁVEIS INTERNAS
//====================================================================================

static constexpr uint32_t TEMPO_LUZ_LIGADA = TEMPO::SEGUNDOS(15);

static CRONOMETRO::Temporizador tempoLuzEstacionamento;

//====================================================================================
//                   SETUP
//====================================================================================

void Controle_Luz_Estacionamento_Inicializa()
{
        debugSetupDivisao(COR::AMARELO, "CONTROLE LUZ ESTACIONAMENTO INICIALIZANDO", COR::MAGENTA, "=", 0);
}

//====================================================================================
//                   LOOP
//====================================================================================

void Controle_Luz_Estacionamento_Atualiza()
{
    bool botaoPressionado = Leitura_Botao_Foi_Pressionado();

    //--------------------------------------------------------
    // DESLIGA SE JÁ ESTIVER TEMPORIZADO
    //--------------------------------------------------------

    if (botaoPressionado &&
        LUZ_ESTACIONAMENTO::atual() == LUZ_ESTACIONAMENTO::ESTADO::TEMPORIZADO)
    {
        LUZ_ESTACIONAMENTO::definir(LUZ_ESTACIONAMENTO::ESTADO::DESLIGADO);
        return;
    }

    //--------------------------------------------------------
    // LIGA TEMPORIZADO
    //--------------------------------------------------------

    if (botaoPressionado)
    {
        tempoLuzEstacionamento.Cronometro_Iniciar();

        LUZ_ESTACIONAMENTO::definir(LUZ_ESTACIONAMENTO::ESTADO::TEMPORIZADO);
    }
}

//====================================================================================
//                   INICIANDO TEMPORIZAÇÃO DA LUZ
//====================================================================================

void Controle_Luz_Estacionamento_Iniciando_Temporizacao()
{

    tempoLuzEstacionamento.Cronometro_Iniciar();

    LUZ_ESTACIONAMENTO::definir(LUZ_ESTACIONAMENTO::ESTADO::TEMPORIZADO);

}

//====================================================================================
//                   LUZ COM TEMPORIZAÇÃO AUTOMÁTICA
//====================================================================================

void Controle_Luz_Estacionamento_Temporizado()
{

     RELE_LUZ_ESTACIONAMENTO.Ligar();

    if (tempoLuzEstacionamento.Cronometro_Dispara_Uma_Vez(TEMPO_LUZ_LIGADA))
    {

        LUZ_ESTACIONAMENTO::definir(LUZ_ESTACIONAMENTO::ESTADO::DESLIGANDO);

    }

}

//====================================================================================
//                   LUZ COM ACIONAMENTO (KAMYLLA)
//====================================================================================

void Controle_Luz_Estacionamento_Forcado()
{

        RELE_LUZ_ESTACIONAMENTO.Ligar();

}

//====================================================================================
//                   LUZ ACIONADA
//====================================================================================

void Controle_Luz_Estacionamento_Acionado()
{

    RELE_LUZ_ESTACIONAMENTO.Ligar();

}

//====================================================================================
//                   LUZ DESLIGADA
//====================================================================================

void Controle_Luz_Estacionamento_Desligando()
{

    RELE_LUZ_ESTACIONAMENTO.Desligar();
    LUZ_ESTACIONAMENTO::definir(LUZ_ESTACIONAMENTO::ESTADO::DESLIGADO);

}

//====================================================================================
//                   LUZ CONSUMINDO (BATERIA)
//====================================================================================

void Controle_Luz_Estacionamento_Consumindo()
{

    RELE_LUZ_ESTACIONAMENTO.Desligar();

}