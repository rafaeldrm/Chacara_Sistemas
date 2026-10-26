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

static bool casaMediaAnterior       = false;
static bool caixaEstoqueAnterior    = false;
//static bool entradaMinimaAnterior   = false;
//static bool entradaMaximaAnterior   = false;

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
    bool boiaCasaMedia = !Leitura_Boias_Esta_Ativa(REGISTRO_BOIAS::BOIA_BIT::CASA_MEDIA);

    debugVariavel("Notificacao CASA_MEDIA estadoAtual: ", boiaCasaMedia, NEGRITO, AMARELO, VERDE);

    if (!TRAVA_BOLEANA::MudouEstado(boiaCasaMedia, casaMediaAnterior))
        return;

    if (boiaCasaMedia)
    {
        debugVariavel("Enviando Mensagem Lora, BOIA MEDIA CASA", boiaCasaMedia, NEGRITO, AMARELO, VERDE);

        LoRa_Enviar_Evento((uint8_t)LORA::ECO_HIDRO::CODIGO::NOTIFICACAO_MAPA_BOIAS_COMPLETO, Leitura_Boias_Mapa_Completo());
    }
}

//====================================================================================
//                              CAIXA ESTOQUE MÁXIMA
//                 FINALIDADE NO DRIMEL SYSTEM: EMERGÊNCIA TRANSBORDAMENTO DE ÁGUA
//====================================================================================

static void Notificacao_Boias_Envio_Transbordamento_Caixa_Estoque()
{
    bool estadoAtual = !Leitura_Boias_Esta_Ativa(REGISTRO_BOIAS::BOIA_BIT::ESTOQUE_MAXIMA);

    if (!TRAVA_BOLEANA::MudouEstado(estadoAtual, caixaEstoqueAnterior))
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
    bool estadoAtual = !Leitura_Boias_Esta_Ativa(REGISTRO_BOIAS::BOIA_BIT::ENTRADA_MINIMA);

    if (!TRAVA_BOLEANA::MudouEstado(estadoAtual, entradaMinimaAnterior))
        return;

    LoRa_Enviar_Evento((uint8_t)LORA::ECO_HIDRO::CODIGO::NOTIFICACAO_MAPA_BOIAS_COMPLETO, Leitura_Boias_Mapa_Completo());
}

//==================================================================================================
//                              CAIXA ENTRADA MÁXIMA
//           FINALIDADE NO DRIMEL SYSTEM: LIBERAR TRANSFERÊNCIA PARA O RESERVATÓRIO (CAIXA ENTRADA)
//==================================================================================================

static void Notificacao_Boias_Envio_Liberar_Transferencia_Caixa_Entrada_Reservatorio()
{
    bool estadoAtual = !Leitura_Boias_Esta_Ativa(REGISTRO_BOIAS::BOIA_BIT::ENTRADA_MAXIMA);

    if (!TRAVA_BOLEANA::MudouEstado(estadoAtual, entradaMaximaAnterior))
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