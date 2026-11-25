//====================================================================================
//                   DEPENDÊNCIAS
//====================================================================================

#include "Notificacao_Boias.h"
#include "DEBUG_MODE.h"

#include "Mudou_Estado.h"
#include "Tempo.h"
#include "Cronometro.h"

#include "Contrato_Boias_Chacara.h"
#include "Leitura_Boias.h"
#include "Envio_Eventos_LoRa.h"
#include "Protocolo_Chacara.h"

//====================================================================================
//                   VARIÁVEIS INTERNAS
//====================================================================================

static bool caixaEstoqueCheioAnterior    = false;
static bool entradaMinimaVaziaAnterior   = false;
static bool entradaMaximaCheiaAnterior   = false;
static bool casaMediaAvisoVaziaAnterior  = false;   // true = Drimel já foi avisado desta situação

// ============ CONTROLE DE ENVIO EMERGENCIAL CAIXA ESTOQUE MAXIMA ===================

//static bool envioTriploAtivo = false;
static uint8_t quantidadeEnvios = 0;

static constexpr uint32_t INTERVALO_ENVIO_EMERGENCIA = TEMPO::SEGUNDOS(6);
static CRONOMETRO::Temporizador tempoUltimoEnvio;
static constexpr uint8_t TOTAL_ENVIOS_EMERGENCIA = 3;

//====================================================================================
//                   SETUP
//====================================================================================

void Notificacao_Boias_Inicializa()
{

 debugSetupDivisao(COR::AMARELO, "NOTIFICAÇÃO BÓIAS INICIALIZANDO", COR::MAGENTA, "=", 0);
}
//====================================================================================
//                              CASA MEDIA 
//                  FINALIDADE NO DRIMEL SYSTEM: ANTECIPAÇÃO, ANÁLISE, DECISÃO
//====================================================================================

static void Notificacao_Boias_Envio_Antecipa_Analise_Bomba_Casa()
{
    bool boiaCasaMediaVazia = !Leitura_Boias_Esta_Ativa(REGISTRO_BOIAS::BOIA_BIT::CASA_MEDIA);

    if (!TRAVA_BOLEANA::MudouEstado(boiaCasaMediaVazia, casaMediaAvisoVaziaAnterior))
        return;
    
    if (boiaCasaMediaVazia)
    {   
    debugAviso("LoRa : Caixa da Casa acabando água ""Casa Media Sinalização"" ");
    LoRa_Enviar_Evento((uint8_t)LORA::ECO_HIDRO::CODIGO::NOTIFICACAO_MAPA_BOIAS_COMPLETO, Leitura_Boias_Mapa_Completo());
    }
}

//====================================================================================
//                              CAIXA ESTOQUE MÁXIMA
//                 FINALIDADE NO DRIMEL SYSTEM: EMERGÊNCIA TRANSBORDAMENTO DE ÁGUA
//====================================================================================
static void Notificacao_Boias_Envio_Transbordamento_Caixa_Estoque()
{
    bool boiaEstoqueMaximaCheia = Leitura_Boias_Esta_Ativa(REGISTRO_BOIAS::BOIA_BIT::ESTOQUE_MAXIMA);

    // Detecta todas as transições.
    bool mudouEstado = TRAVA_BOLEANA::MudouEstado(boiaEstoqueMaximaCheia, caixaEstoqueCheioAnterior);

    // Inicia a sequência somente na entrada em CHEIA.
    if (mudouEstado && boiaEstoqueMaximaCheia)
    {
        quantidadeEnvios = 0;
        tempoUltimoEnvio.Cronometro_Iniciar();
    }

    // Cancela a sequência se a caixa deixar de estar cheia.
    if (!boiaEstoqueMaximaCheia)
    {
        quantidadeEnvios = 0;
        return;
    }

    if (quantidadeEnvios >= TOTAL_ENVIOS_EMERGENCIA)
        return;

    if (tempoUltimoEnvio.Cronometro_Dispara_Uma_Vez(INTERVALO_ENVIO_EMERGENCIA))
    {
        debugAviso("LoRa : EMERGÊNCIA Transbordamento : Caixa Estoque Máxima Cheia");

        LoRa_Enviar_Evento(
            (uint8_t)LORA::ECO_HIDRO::CODIGO::NOTIFICACAO_MAPA_BOIAS_COMPLETO,
            Leitura_Boias_Mapa_Completo()
        );

        quantidadeEnvios++;

        // Prepara o próximo intervalo.
        if (quantidadeEnvios < TOTAL_ENVIOS_EMERGENCIA)
            tempoUltimoEnvio.Cronometro_Iniciar();
    }
}

//==============================================================================================================
//                              CAIXA ENTRADA MÍNIMA
//                FINALIDADE NO DRIMEL SYSTEM: ADICIONAR CLORO E FECHAR SAÍDA E ABRIR A ENTRADA (CAIXA ENTRADA)
//==============================================================================================================

static void Notificacao_Boias_Envio_Adiciona_Cloro_Caixa_Entrada()
{
    bool boiaEntradaMinimaVazia = !Leitura_Boias_Esta_Ativa(REGISTRO_BOIAS::BOIA_BIT::ENTRADA_MINIMA);

    if (!TRAVA_BOLEANA::MudouEstado(boiaEntradaMinimaVazia, entradaMinimaVaziaAnterior))
        return;

    if (boiaEntradaMinimaVazia)
    {    
    debugAviso("LoRa : Adicionar Cloro : Caixa Entrada Mínima ""Vazia"" ");
    LoRa_Enviar_Evento((uint8_t)LORA::ECO_HIDRO::CODIGO::NOTIFICACAO_MAPA_BOIAS_COMPLETO, Leitura_Boias_Mapa_Completo());
    }
}

//==================================================================================================
//                              CAIXA ENTRADA MÁXIMA
//           FINALIDADE NO DRIMEL SYSTEM: LIBERAR TRANSFERÊNCIA PARA O RESERVATÓRIO (CAIXA ENTRADA)
//==================================================================================================

static void Notificacao_Boias_Envio_Liberar_Transferencia_Caixa_Entrada_Reservatorio()
{
    bool boiaEntradaMaximaCheia = Leitura_Boias_Esta_Ativa(REGISTRO_BOIAS::BOIA_BIT::ENTRADA_MAXIMA);

    if (!TRAVA_BOLEANA::MudouEstado(boiaEntradaMaximaCheia, entradaMaximaCheiaAnterior))
        return;
    
    if (boiaEntradaMaximaCheia)
    {    
    debugAviso("LoRa : Pronto para Transferir para os Reservatórios : Caixa Entrada ""Cheia"" ");
    LoRa_Enviar_Evento((uint8_t)LORA::ECO_HIDRO::CODIGO::NOTIFICACAO_MAPA_BOIAS_COMPLETO, Leitura_Boias_Mapa_Completo());
    }
}

//====================================================================================
//                   LOOP
//====================================================================================

void Notificacao_Boias_Atualiza()
{
    Notificacao_Boias_Envio_Antecipa_Analise_Bomba_Casa();
    Notificacao_Boias_Envio_Transbordamento_Caixa_Estoque();
    Notificacao_Boias_Envio_Adiciona_Cloro_Caixa_Entrada();
    Notificacao_Boias_Envio_Liberar_Transferencia_Caixa_Entrada_Reservatorio();
}