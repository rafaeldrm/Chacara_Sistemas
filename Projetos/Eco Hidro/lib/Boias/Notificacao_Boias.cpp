//====================================================================================
//                   DEPENDÊNCIAS
//====================================================================================

#include "Notificacao_Boias.h"
#include "DEBUG_MODE.h"

#include "Mudou_Estado.h"

#include "Contrato_Boias_Chacara.h"
#include "Leitura_Boias.h"
#include "Envio_Eventos_LoRa.h"
#include "Protocolo_Chacara.h"

//====================================================================================
//                   VARIÁVEIS INTERNAS
//====================================================================================

static bool casaMediaVaziaAnterior       = false;
static bool caixaEstoqueCheioAnterior    = false;
//static bool entradaMinimaVaziaAnterior   = false;
//static bool entradaMaximaCheiaAnterior   = false;

static bool casaMediaVaziaAvisada = false;   // true = Drimel já foi avisado desta situação

//====================================================================================
//                   SETUP
//====================================================================================

void Notificacao_Boias_Inicializa()
{

 debugSetupDivisao(COR::AMARELO, "NOTIFICAÇÃO BÓIAS INICIALIZANDO", COR::MAGENTA, "=", 45);
}
//====================================================================================
//                              CASA MEDIA 
//                  FINALIDADE NO DRIMEL SYSTEM: ANTECIPAÇÃO, ANÁLISE, DECISÃO
//====================================================================================

static void Notificacao_Boias_Envio_Antecipa_Analise_Bomba_Casa()
{
    const bool casaMediaVazia = !Leitura_Boias_Esta_Ativa(REGISTRO_BOIAS::BOIA_BIT::CASA_MEDIA);

    //--------------------------------------------------------
    // ENCHEU DE NOVO: libera um novo aviso no futuro
    //--------------------------------------------------------

    if (!casaMediaVazia)
    {
        casaMediaVaziaAvisada = false;
        return;
    }

    //--------------------------------------------------------
    // JÁ AVISOU ESTA SITUAÇÃO: não repete
    //--------------------------------------------------------

    if (casaMediaVaziaAvisada)
        return;

    //--------------------------------------------------------
    // TENTA AVISAR: só marca como avisada se entrou na fila
    //--------------------------------------------------------

    if (LoRa_Enviar_Evento((uint8_t)LORA::ECO_HIDRO::CODIGO::NOTIFICACAO_MAPA_BOIAS_COMPLETO,
                           Leitura_Boias_Mapa_Completo()))
    {
        casaMediaVaziaAvisada = true;
    }
}

//====================================================================================
//                              CAIXA ESTOQUE MÁXIMA
//                 FINALIDADE NO DRIMEL SYSTEM: EMERGÊNCIA TRANSBORDAMENTO DE ÁGUA
//====================================================================================

static void Notificacao_Boias_Envio_Transbordamento_Caixa_Estoque()
{
    bool boiaEstoqueMaximaCheia = !Leitura_Boias_Esta_Ativa(REGISTRO_BOIAS::BOIA_BIT::ESTOQUE_MAXIMA);

    if (!TRAVA_BOLEANA::MudouEstado(boiaEstoqueMaximaCheia, caixaEstoqueCheioAnterior))
        return;

    LoRa_Enviar_Evento((uint8_t)LORA::ECO_HIDRO::CODIGO::NOTIFICACAO_MAPA_BOIAS_COMPLETO, Leitura_Boias_Mapa_Completo());
}

//==============================================================================================================
//                              CAIXA ENTRADA MÍNIMA
//                FINALIDADE NO DRIMEL SYSTEM: ADICIONAR CLORO E FECHAR SAÍDA E ABRIR A ENTRADA (CAIXA ENTRADA)
//==============================================================================================================
/*
static void Notificacao_Boias_Envio_Adiciona_Cloro_Caixa_Entrada()
{
    bool boiaEntradaMinimaVazia = !Leitura_Boias_Esta_Ativa(REGISTRO_BOIAS::BOIA_BIT::ENTRADA_MINIMA);

    if (!TRAVA_BOLEANA::MudouEstado(boiaEntradaMinimaVazia, entradaMinimaVaziaAnterior))
        return;

    LoRa_Enviar_Evento((uint8_t)LORA::ECO_HIDRO::CODIGO::NOTIFICACAO_MAPA_BOIAS_COMPLETO, Leitura_Boias_Mapa_Completo());
}

//==================================================================================================
//                              CAIXA ENTRADA MÁXIMA
//           FINALIDADE NO DRIMEL SYSTEM: LIBERAR TRANSFERÊNCIA PARA O RESERVATÓRIO (CAIXA ENTRADA)
//==================================================================================================

static void Notificacao_Boias_Envio_Liberar_Transferencia_Caixa_Entrada_Reservatorio()
{
    bool boiaEntradaMaximaCheia = !Leitura_Boias_Esta_Ativa(REGISTRO_BOIAS::BOIA_BIT::ENTRADA_MAXIMA);

    if (!TRAVA_BOLEANA::MudouEstado(boiaEntradaMaximaCheia, entradaMaximaCheiaAnterior))
        return;

    LoRa_Enviar_Evento((uint8_t)LORA::ECO_HIDRO::CODIGO::NOTIFICACAO_MAPA_BOIAS_COMPLETO, Leitura_Boias_Mapa_Completo());
}
*/
//====================================================================================
//                   LOOP
//====================================================================================

void Notificacao_Boias_Atualiza()
{

    Notificacao_Boias_Envio_Antecipa_Analise_Bomba_Casa();
    Notificacao_Boias_Envio_Transbordamento_Caixa_Estoque();
 //   Notificacao_Boias_Envio_Adiciona_Cloro_Caixa_Entrada();
 //   Notificacao_Boias_Envio_Liberar_Transferencia_Caixa_Entrada_Reservatorio();

}