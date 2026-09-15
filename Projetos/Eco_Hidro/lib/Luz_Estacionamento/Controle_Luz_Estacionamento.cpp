//====================================================================================
//                   DEPENDÊNCIAS
//====================================================================================

#include "Controle_Luz_Estacionamento.h"
#include "DEBUG_MODE.h"

#include "Leitura_Botao.h"
#include "Modulo_Reles.h"

#include "Tempo.h"
#include "Cronometro.h"
#include "Estados_Sistema_Eco_Hidro.h"

//====================================================================================
//                   VARIÁVEIS INTERNAS
//====================================================================================

static constexpr uint32_t TEMPO_LUZ_LIGADA = TEMPO::MINUTOS(15);

static CRONOMETRO::Temporizador tempoLuzEstacionamento;

//====================================================================================
//                   SETUP
//====================================================================================

void Controle_Luz_Estacionamento_Inicializa()
{

    tempoLuzEstacionamento.Cronometro_Iniciar();

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
        LUZ_ESTACIONAMENTO::atual() == LUZ_ESTACIONAMENTO::SITUACAO::TEMPORIZADO)
    {
        LUZ_ESTACIONAMENTO::definir(LUZ_ESTACIONAMENTO::SITUACAO::DESLIGADO);
        return;
    }

    //--------------------------------------------------------
    // LIGA TEMPORIZADO
    //--------------------------------------------------------

    if (botaoPressionado)
    {
        tempoLuzEstacionamento.Cronometro_Iniciar();

        LUZ_ESTACIONAMENTO::definir(
            LUZ_ESTACIONAMENTO::SITUACAO::TEMPORIZADO
        );
    }
}
//====================================================================================
//                   LUZ COM TEMPORIZAÇÃO AUTOMÁTICA
//====================================================================================

void Controle_Luz_Estacionamento_Temporizado()
{

     RELE_LUZ_ESTACIONAMENTO.Ligar();

    if (tempoLuzEstacionamento.Cronometro_Dispara_Uma_Vez(TEMPO_LUZ_LIGADA))
    {

        LUZ_ESTACIONAMENTO::definir(LUZ_ESTACIONAMENTO::SITUACAO::DESLIGANDO);

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
//                   LUZ DESLIGADA
//====================================================================================

void Controle_Luz_Estacionamento_Desligando()
{

    RELE_LUZ_ESTACIONAMENTO.Desligar();
    LUZ_ESTACIONAMENTO::definir(LUZ_ESTACIONAMENTO::SITUACAO::DESLIGADO);

}

//====================================================================================
//                   LUZ CONSUMINDO (BATERIA)
//====================================================================================

void Controle_Luz_Estacionamento_Consumindo()
{

    RELE_LUZ_ESTACIONAMENTO.Desligar();

}