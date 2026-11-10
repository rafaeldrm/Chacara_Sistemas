//====================================================================================
//                   DEPENDÊNCIAS
//====================================================================================

#include "Mediador_Sistema.h"
#include "DEBUG_MODE.h"

#include "Estados_Sistema_Eco_Hidro.h"
#include "Estados_Eventos_Eco_Hidro.h"

#include "Mediador_Casa_Torre.h"
#include "Mediador_Reservatorio.h"
#include "Mediador_Luz_Estacionamento.h"
#include "Mediador_Luz_Acesso_Reservatorio.h"
#include "Mediador_Voltagem.h"
#include "Mediador_Unidade_Potencia.h"

//====================================================================================
//                   VARIÁVEIS INTERNAS
//====================================================================================

//====================================================================================
//                   SETUP
//====================================================================================

void MEDIADOR_BOOT()
{
    debugSetupArquivo();
    debugSetupDivisao(COR::AMARELO, "MEDIADOR INICIALIZANDO", COR::MAGENTA, "=", 45);

    Mediador_Casa_Torre_Inicializa();

    Mediador_Reservatorio_Inicializa();

    Mediador_Voltagem_Inicializa();

    Mediador_Luz_Estacionamento_Inicializa();

    Mediador_Luz_Acesso_Reservatorio_Inicializa();

    Mediador_Unidade_Potencia_Inicializa();

    //--------------------------------------------------------------------------------
    //  ESTADOS INICIAIS
    //--------------------------------------------------------------------------------

    SISTEMA_ECO_HIDRO::definir(SISTEMA_ECO_HIDRO::MODO::NORMAL);
    POTENCIA::definir(POTENCIA::SELECIONADO::REPOUSO);
    DESLIGAMENTO_BOMBA::definir(DESLIGAMENTO_BOMBA::MOTIVO::NENHUM);
}

//====================================================================================
//                   LOOP
//====================================================================================

void MEDIADOR_LOOP()
{
    Mediador_Casa_Torre_Atualiza();

    Mediador_Reservatorio_Atualiza();

    Mediador_Voltagem_Atualiza();

    Mediador_Luz_Acesso_Reservatorio_Atualiza();

    Mediador_Luz_Estacionamento_Atualiza();

    Mediador_Unidade_Potencia_Atualiza();

}