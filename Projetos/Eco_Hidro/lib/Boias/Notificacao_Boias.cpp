//====================================================================================
//                   DEPENDÊNCIAS
//====================================================================================

#include "Notificacao_Boias.h"
#include "DEBUG_MODE.h"
#include "Transicao_Boleana.h"

#include "Boias_ID.h"
#include "Leitura_Boias.h"
#include "Gerenciador_Mensagens.h"
#include "Protocolo_Chacara.h"

//====================================================================================
//                   VARIÁVEIS INTERNAS
//====================================================================================

static void Notificacao_Boias_Envio_Antecipa_Analise_Bomba_Casa();
static void Notificacao_Boias_Envio_Transbordamento_Caixa_Estoque();
static void Notificacao_Boias_Envio_Adiciona_Cloro_Caixa_Entrada();
static void Notificacao_Boias_Envio_Liberar_Transferencia_Caixa_Entrada_Reservatorio();

static bool casaMediaAnterior       = false;
static bool caixaEstoqueAnterior    = false;
static bool entradaMinimaAnterior   = false;
static bool entradaMaximaAnterior   = false;

//====================================================================================
//                   SETUP
//====================================================================================

void Notificacao_Boias_Inicializa()
{



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

//====================================================================================
//                              CASA MEDIA 
//                  FINALIDADE NO DRIMEL SYSTEM: ANTECIPAÇÃO, ANÁLISE, DECISÃO
//====================================================================================

static void Notificacao_Boias_Envio_Antecipa_Analise_Bomba_Casa()
{
    bool estadoAtual =
        !Leitura_Boias_Esta_Ativa(REGISTRO_BOIAS::BOIA_BIT::CASA_MEDIA);

    if (!MudouEstado(estadoAtual, casaMediaAnterior))
        return;

    LoRa_Enviar_Evento(
        (uint8_t)LORA::ECO_HIDRO::CODIGO::NIVEL_BOIAS,
        Leitura_Boias_Mapa_Completo()
    );
}

//====================================================================================
//                              CAIXA ESTOQUE MÁXIMA
//                 FINALIDADE NO DRIMEL SYSTEM: EMERGÊNCIA TRANSBORDAMENTO DE ÁGUA
//====================================================================================

static void Notificacao_Boias_Envio_Transbordamento_Caixa_Estoque()
{
    bool estadoAtual =
        !Leitura_Boias_Esta_Ativa(REGISTRO_BOIAS::BOIA_BIT::ESTOQUE_MAXIMA);

    if (!MudouEstado(estadoAtual, caixaEstoqueAnterior))
        return;

    LoRa_Enviar_Evento(
        (uint8_t)LORA::ECO_HIDRO::CODIGO::NIVEL_BOIAS,
        Leitura_Boias_Mapa_Completo()
    );
}

//==============================================================================================================
//                              CAIXA ENTRADA MÍNIMA
//                FINALIDADE NO DRIMEL SYSTEM: ADICIONAR CLORO E FECHAR SAÍDA E ABRIR A ENTRADA (CAIXA ENTRADA)
//==============================================================================================================

static void Notificacao_Boias_Envio_Adiciona_Cloro_Caixa_Entrada()
{
    bool estadoAtual =
        !Leitura_Boias_Esta_Ativa(REGISTRO_BOIAS::BOIA_BIT::ENTRADA_MINIMA);

    if (!MudouEstado(estadoAtual, entradaMinimaAnterior))
        return;

    LoRa_Enviar_Evento(
        (uint8_t)LORA::ECO_HIDRO::CODIGO::NIVEL_BOIAS,
        Leitura_Boias_Mapa_Completo()
    );
}

//==================================================================================================
//                              CAIXA ENTRADA MÁXIMA
//           FINALIDADE NO DRIMEL SYSTEM: LIBERAR TRANSFERÊNCIA PARA O RESERVATÓRIO (CAIXA ENTRADA)
//==================================================================================================

static void Notificacao_Boias_Envio_Liberar_Transferencia_Caixa_Entrada_Reservatorio()
{
    bool estadoAtual =
        !Leitura_Boias_Esta_Ativa(REGISTRO_BOIAS::BOIA_BIT::ENTRADA_MAXIMA);

    if (!MudouEstado(estadoAtual, entradaMaximaAnterior))
        return;

    LoRa_Enviar_Evento(
        (uint8_t)LORA::ECO_HIDRO::CODIGO::NIVEL_BOIAS,
        Leitura_Boias_Mapa_Completo()
    );
}