
 //====================================================================================
 //                   DEPENDÊNCIAS
 //====================================================================================

#include "Mediador_Sistema.h"
#include "DEBUG_MODE.h"

#include "Configuracao_Subsistemas.h"

#include "Estados_Sistema_Eco_Hidro.h"
#include "Mediador_Casa_Torre.h"
#include "Mediador_Bomba_Casa.h"
#include "Mediador_Reservatorio.h"
#include "Mediador_Bomba_12v.h"
#include "Mediador_Voltagem.h"
#include "Mediador_Luz_Estacionamento.h"
#include "Mediador_Luz_Acesso_Reservatorio.h"
#include "Mediador_Unidade_Potencia.h"
#include "Mediador_Potencia.h"
#include "Termico_Bomba_Casa/Mediador_Termico_Bomba_Casa.h"
#include "Giro_Preventivo_Bomba_Casa/Mediador_Giro_Preventivo_Bomba_Casa.h"
#include "Movimento_Bomba_Casa/Mediador_Tranco_Bomba_Casa.h"


//====================================================================================
//                   VARIÁVEIS INTERNAS
//====================================================================================


//====================================================================================
//                       SETUP
//====================================================================================

void MEDIADOR_BOOT()
{
    debugSetupArquivo();
    debugSetupDivisao(COR::AMARELO, "MEDIADOR INICIALIZANDO", COR::MAGENTA, "=", 0);

    Mediador_Casa_Torre_Inicializa();

    Mediador_Bomba_Casa_Inicializa();

#if ATIVAR_RESERVATORIO
    Mediador_Reservatorio_Inicializa();
#endif

    Mediador_Bomba_12v_Inicializa();

#if ATIVAR_VOLTAGEM
    Mediador_Voltagem_Inicializa();
#endif

#if ATIVAR_LUZ_ESTACIONAMENTO
    Mediador_Luz_Estacionamento_Inicializa();
#endif

#if ATIVAR_LUZ_ACESSO_RESERVATORIO
    Mediador_Luz_Acesso_Reservatorio_Inicializa();
#endif

    Mediador_Potencia_Inicializa();

    Mediador_Unidade_Potencia_Inicializa();

#if ATIVAR_TERMICO_BOMBA_CASA
    Mediador_Termico_Bomba_Casa_Inicializa();
#endif

#if ATIVAR_GIRO_PREVENTIVO_BOMBA_CASA
    Mediador_Giro_Preventivo_Bomba_Casa_Inicializa();
#endif

#if ATIVAR_TRANCO_BOMBA_CASA
    Mediador_Tranco_Bomba_Casa_Inicializa();
#endif


    //--------------------------------------------------------------------------------
    //                       ESTADOS INICIAIS
    //--------------------------------------------------------------------------------

    SISTEMA_ECO_HIDRO::definir(SISTEMA_ECO_HIDRO::ESTADO::NORMAL);

    POTENCIA::definir(POTENCIA::ESTADO::REPOUSO);

    DESLIGAMENTO_BOMBA::definir(DESLIGAMENTO_BOMBA::ESTADO::NENHUM);
}


//====================================================================================
//                        LOOP
//====================================================================================

void MEDIADOR_LOOP()
{
    Mediador_Casa_Torre_Atualiza();

    Mediador_Bomba_Casa_Atualiza();

#if ATIVAR_RESERVATORIO
    Mediador_Reservatorio_Atualiza();

    Mediador_Bomba_12v_Atualiza();
#endif

#if ATIVAR_VOLTAGEM
    Mediador_Voltagem_Atualiza();
#endif

#if ATIVAR_LUZ_ACESSO_RESERVATORIO
    Mediador_Luz_Acesso_Reservatorio_Atualiza();
#endif

#if ATIVAR_LUZ_ESTACIONAMENTO
    Mediador_Luz_Estacionamento_Atualiza();
#endif

    Mediador_Potencia_Atualiza();

    Mediador_Unidade_Potencia_Atualiza();

#if ATIVAR_TERMICO_BOMBA_CASA
    Mediador_Termico_Bomba_Casa_Atualiza();
#endif

#if ATIVAR_GIRO_PREVENTIVO_BOMBA_CASA
    Mediador_Giro_Preventivo_Bomba_Casa_Atualiza();
#endif

#if ATIVAR_TRANCO_BOMBA_CASA
    Mediador_Tranco_Bomba_Casa_Atualiza();
#endif
}
