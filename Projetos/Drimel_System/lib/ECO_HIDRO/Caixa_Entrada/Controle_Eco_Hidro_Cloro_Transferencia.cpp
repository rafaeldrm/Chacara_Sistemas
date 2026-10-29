//====================================================================================
//                   DEPENDÊNCIAS
//====================================================================================

#include "Controle_Eco_Hidro_Cloro_Transferencia.h"
#include "DEBUG_MODE.h"

#include "Tempo.h"
#include "Cronometro.h"

#include "Contrato_Boias_Chacara.h"
#include "Boias\Leitura_Eco_Hidro_Boias.h"

#include "Estado_Sistema_Drimel_System.h"

//====================================================================================
//                   VARIÁVEIS INTERNAS
//====================================================================================

// ATENÇÃO: "ativa" = contato fechado = ÁGUA PRESENTE. "Vazia" = NÃO ativa.

static constexpr uint32_t TEMPO_LEMBRETE_CLORO    = TEMPO::MINUTOS(30);   // chute, ajustar
static constexpr uint32_t TEMPO_ENCHIMENTO_MAXIMO = TEMPO::HORAS(6);      // chute, ajustar

static CRONOMETRO::Temporizador lembreteCloro;

static CRONOMETRO::Temporizador enchimentoCaixa;

static bool pedidoConfirmarCloro = false;

//====================================================================================
//                   AUXILIARES (LEITURA DAS BÓIAS)
//====================================================================================

static bool Entrada_Minima_Vazia()
{
    return !Leitura_Eco_Hidro_Boias_Esta_Ativa(REGISTRO_BOIAS::BOIA_BIT::ENTRADA_MINIMA);
}



static bool Entrada_Maxima_Cheia()
{
    return Leitura_Eco_Hidro_Boias_Esta_Ativa(REGISTRO_BOIAS::BOIA_BIT::ENTRADA_MAXIMA);
}

//====================================================================================
//                   SETUP
//====================================================================================

void Controle_Eco_Hidro_Cloro_Transferencia_Inicializa()
{
    pedidoConfirmarCloro = false;
}

//====================================================================================
//                   CONFIRMAR CLORO (QUEM CHAMA: BOTÃO, TELA, VOZ, TELEGRAM)
//====================================================================================

void Controle_Eco_Hidro_Cloro_Transferencia_Confirmar_Cloro()
{
    pedidoConfirmarCloro = true;
}

//====================================================================================
//                   DESCONHECIDO (espera uma bóia definitiva)
//====================================================================================

void Controle_Eco_Hidro_Cloro_Transferencia_Desconhecido()
{
    if (Entrada_Minima_Vazia())
    {
        ECO_HIDRO::CAIXA_ENTRADA::definir(ECO_HIDRO::CAIXA_ENTRADA::SITUACAO::ADICIONAR_CLORO);
        return;
    }

    if (Entrada_Maxima_Cheia())
    {
        ECO_HIDRO::CAIXA_ENTRADA::definir(ECO_HIDRO::CAIXA_ENTRADA::SITUACAO::INICIAR_TRANSFERENCIA);
        return;
    }

    // Meio do caminho: não sei em que ponto do ciclo estou, espero.
}

//====================================================================================
//                   ADICIONAR CLORO (ação única: avisa e passa adiante)
//====================================================================================

void Controle_Eco_Hidro_Cloro_Transferencia_Adicionar_Cloro()
{
    pedidoConfirmarCloro = false;                 // limpa confirmação "velha"

    lembreteCloro.Cronometro_Iniciar();

    //EVENTO::RESERVATORIO::definir(EVENTO::RESERVATORIO::MENSAGEM::CAIXA_ENTRADA_CLORO);

    ECO_HIDRO::CAIXA_ENTRADA::definir(ECO_HIDRO::CAIXA_ENTRADA::SITUACAO::AVISADO_CLORO);
}

//====================================================================================
//                   AVISADO CLORO (espera você confirmar, e fica lembrando)
//====================================================================================

void Controle_Eco_Hidro_Cloro_Transferencia_Avisado_Cloro()
{
    if (pedidoConfirmarCloro)
    {
        pedidoConfirmarCloro = false;

        enchimentoCaixa.Cronometro_Iniciar();

        ECO_HIDRO::CAIXA_ENTRADA::definir(ECO_HIDRO::CAIXA_ENTRADA::SITUACAO::CLORO_CONFIRMADO);
        return;
    }

    // Encheu sem a confirmação do cloro: não trava o ciclo (decidir depois se avisa diferente)
    if (Entrada_Maxima_Cheia())
    {
        ECO_HIDRO::CAIXA_ENTRADA::definir(ECO_HIDRO::CAIXA_ENTRADA::SITUACAO::INICIAR_TRANSFERENCIA);
        return;
    }

    if (lembreteCloro.Cronometro_Ciclo_Continuo(TEMPO_LEMBRETE_CLORO))
    {
        //EVENTO::RESERVATORIO::definir(EVENTO::RESERVATORIO::MENSAGEM::CAIXA_ENTRADA_CLORO);
    }
}

//====================================================================================
//                   CLORO CONFIRMADO (esperando encher; vigia a demora)
//====================================================================================

void Controle_Eco_Hidro_Cloro_Transferencia_Cloro_Confirmado()
{
    if (Entrada_Maxima_Cheia())
    {
        ECO_HIDRO::CAIXA_ENTRADA::definir(ECO_HIDRO::CAIXA_ENTRADA::SITUACAO::INICIAR_TRANSFERENCIA);
        return;
    }

    if (enchimentoCaixa.Cronometro_Dispara_Uma_Vez(TEMPO_ENCHIMENTO_MAXIMO))
    {
        //EVENTO::RESERVATORIO::definir(EVENTO::RESERVATORIO::MENSAGEM::CAIXA_ENTRADA_ENCHIMENTO_DEMORANDO);
    }
}

//====================================================================================
//                   INICIAR TRANSFERÊNCIA (ação única: avisa e passa adiante)
//====================================================================================

void Controle_Eco_Hidro_Cloro_Transferencia_Iniciar_Transferencia()
{
    enchimentoCaixa.Cronometro_Cancelar();

   // EVENTO::RESERVATORIO::definir(EVENTO::RESERVATORIO::MENSAGEM::CAIXA_ENTRADA_TRANSFERENCIA);

    ECO_HIDRO::CAIXA_ENTRADA::definir(ECO_HIDRO::CAIXA_ENTRADA::SITUACAO::AVISADO_TRANSFERENCIA);
}

//====================================================================================
//                   AVISADO TRANSFERÊNCIA (esperando a caixa esvaziar)
//====================================================================================

void Controle_Eco_Hidro_Cloro_Transferencia_Avisado_Transferencia()
{
    if (Entrada_Minima_Vazia())
    {
        ECO_HIDRO::CAIXA_ENTRADA::definir(ECO_HIDRO::CAIXA_ENTRADA::SITUACAO::CONCLUIDO);
    }
}

//====================================================================================
//                   CONCLUÍDO (máxima -> mínima: soma os 2.000 L e recomeça)
//====================================================================================

void Controle_Eco_Hidro_Cloro_Transferencia_Concluido()
{
    // Registro_Eco_Hidro_Litragem_Somar_Ciclo();   // próxima camada a criar

    ECO_HIDRO::CAIXA_ENTRADA::definir(ECO_HIDRO::CAIXA_ENTRADA::SITUACAO::ADICIONAR_CLORO);
}