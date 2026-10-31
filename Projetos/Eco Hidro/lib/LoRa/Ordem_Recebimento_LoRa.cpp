//====================================================================================
//                   DEPENDÊNCIAS
//====================================================================================

#include "Ordem_Recebimento_LoRa.h"
#include "DEBUG_MODE.h"

#include "Endereco_LoRa.h"

#include "Estados_Sistema_Eco_Hidro.h"
#include "Estados_Eventos_Eco_Hidro.h"
#include "Envio_Eventos_LoRa.h"

//====================================================================================
//                   VARIÁVEIS INTERNAS
//====================================================================================

// Quando o Drimel não recebe o ACK, ele reenvia a mesma ordem. Aqui guardamos o ACK
// da última ordem para poder repetir, sem executar a ordem de novo.
static uint8_t  ultimaResposta = 0;     // 0 = a última ordem não pedia ACK
static uint32_t ultimaRespostaMs = 0;

// As 3 cópias de uma mesma transmissão chegam dentro dessa janela. Uma repetição
// depois dela é um reenvio do Drimel (ACK perdido), e não só mais uma cópia.
static constexpr uint32_t JANELA_COPIAS_MS = 2500;

//====================================================================================
//                   SETUP
//====================================================================================

void Ordem_Recebimento_Lora_Inicializa()
{
        debugSetupDivisao(COR::AMARELO, "ORDEM RECEBIMENTO LORA INICIALIZANDO", COR::MAGENTA, "=", 45);
}

//====================================================================================
//                   RESPONDE UMA ORDEM (ACK)
//====================================================================================

static void ResponderOrdem(LORA::ECO_HIDRO::CODIGO resposta)
{
    ultimaResposta = (uint8_t)resposta;
    ultimaRespostaMs = millis();

    LoRa_Enviar_Evento(ultimaResposta, 0);
}

//====================================================================================
//                   RECEBE MENSAGENS DO SISTEMA RESIDENCIAL
//====================================================================================

bool Ordem_Recebimento_LoRa(const LORA::Pacote& pacote)
{
    ultimaResposta = 0;   // ordem nova: o ACK da anterior não vale mais



    switch ((LORA::ECO_HIDRO::CODIGO)pacote.codigo)
    {

        case LORA::ECO_HIDRO::CODIGO::ORDEM_CICLO_COMPLETO:
            ORDENS_DRIMEL_SYSTEM::CASA_TORRE::definir(ORDENS_DRIMEL_SYSTEM::CASA_TORRE::ORDEM::CICLO_COMPLETO);
            ResponderOrdem(LORA::ECO_HIDRO::CODIGO::RESPOSTA_ORDEM_CICLO_COMPLETO);
            return true;



        case LORA::ECO_HIDRO::CODIGO::ORDEM_MEIO_CICLO:
            ORDENS_DRIMEL_SYSTEM::CASA_TORRE::definir(ORDENS_DRIMEL_SYSTEM::CASA_TORRE::ORDEM::MEIO_CICLO);
            ResponderOrdem(LORA::ECO_HIDRO::CODIGO::RESPOSTA_ORDEM_MEIO_CICLO);
            return true;



        case LORA::ECO_HIDRO::CODIGO::ORDEM_DESLIGAR_BOMBA_CASA:
            ORDENS_DRIMEL_SYSTEM::CASA_TORRE::definir(ORDENS_DRIMEL_SYSTEM::CASA_TORRE::ORDEM::DESLIGA);
            return true;



        case LORA::ECO_HIDRO::CODIGO::ORDEM_NORMAL:
            ORDENS_DRIMEL_SYSTEM::CASA_TORRE::definir(ORDENS_DRIMEL_SYSTEM::CASA_TORRE::ORDEM::NENHUM);
            return true;



        //outras mensagens LoRa a serem adicionados aqui

        default:
            return false; // não resolveu nada importante
    }
}

//====================================================================================
//                   REPETE O ACK SE O DRIMEL REENVIOU A ORDEM
//====================================================================================

static void ReenviarRespostaSeNecessario()
{
    if (ultimaResposta == 0)
        return;

    if ((uint32_t)(millis() - ultimaRespostaMs) <= JANELA_COPIAS_MS)
        return;   // ainda é só mais uma cópia da mesma transmissão



    debugAviso("Ordem_Recebimento: ordem reenviada pelo Drimel, repetindo o ACK");

    ResponderOrdem((LORA::ECO_HIDRO::CODIGO)ultimaResposta);
}



//====================================================================================
//                   LOOP
//====================================================================================

void Ordem_Recebimento_LoRa_Atualiza()
{
    LORA::Pacote pacote;
    bool repetida = false;

    if (Endereco_LoRa_Receber(pacote, &repetida))
    {
        Ordem_Recebimento_LoRa(pacote);
        return;
    }

    if (repetida)
        ReenviarRespostaSeNecessario();
}