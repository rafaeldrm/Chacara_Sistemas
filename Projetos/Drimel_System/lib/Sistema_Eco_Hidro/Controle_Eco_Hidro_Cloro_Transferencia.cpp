//====================================================================================
//                   DEPENDÊNCIAS
//====================================================================================

#include "Controle_Eco_Hidro_Cloro_Transferencia.h"
#include "DEBUG_MODE.h"

#include "Leitura_Eco_Hidro_Boias.h"
#include "Boias_ID.h"

#include "Estado_Sistema_Drimel_System.h"

//====================================================================================
//                   VARIÁVEIS INTERNAS
//====================================================================================

static bool entradaMaximaCheia = false;
static bool entradaMinimaVazia = false;

//====================================================================================
//                   SETUP
//====================================================================================

void Controle_Eco_Hidro_Cloro_Transferencia_Inicializa()
{



}

//====================================================================================
//                   ATUALIZA BOIAS
//====================================================================================

static void Controle_Caixa_Entrada_Atualiza_Boias()
{

    entradaMaximaCheia = Leitura_Boias_Esta_Ativa(REGISTRO_BOIAS::BOIA_BIT::ENTRADA_MAXIMA);

    entradaMinimaVazia = !Leitura_Boias_Esta_Ativa(REGISTRO_BOIAS::BOIA_BIT::ENTRADA_MINIMA);

}

//====================================================================================
//                   ADICIONA CLORO
//====================================================================================

void Controle_Eco_Hidro_Cloro_Transferencia_Normal()
{

  Controle_Caixa_Entrada_Atualiza_Boias();

  if (entradaMinimaVazia)
  {

    //Mensagem aviso (Aplicar 4 Gramas de Cloro, ou o Mediador fará essa logística de comunicação)
    ECO_HIDRO::CAIXA_ENTRADA::definir(ECO_HIDRO::CAIXA_ENTRADA::SITUACAO::ADICIONAR_CLORO);

  }

  if (entradaMaximaCheia)
  {
    //Mensagem aviso (esvazie a caixa da entrada), o Mediador provavelmente fará essa atribuição com o módulo de audio.
    ECO_HIDRO::CAIXA_ENTRADA::definir(ECO_HIDRO::CAIXA_ENTRADA::SITUACAO::INICIAR_TRANSFERENCIA);

  }

}

//====================================================================================
//                   ADICIONAR CLORO
//====================================================================================

void Controle_Eco_Hidro_Cloro_Transferencia_Adicionar_Cloro()
{

  //Mensagem aviso (Aplicar 4 Gramas de Cloro, ou o Mediador fará essa logística de comunicação)
  ECO_HIDRO::CAIXA_ENTRADA::definir(ECO_HIDRO::CAIXA_ENTRADA::SITUACAO::AVISADO_CLORO);

}
