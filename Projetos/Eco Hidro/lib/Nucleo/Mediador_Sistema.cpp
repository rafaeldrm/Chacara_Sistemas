//====================================================================================
//                   DEPENDÊNCIAS
//====================================================================================

#include "Mediador_Sistema.h"
#include "DEBUG_MODE.h"

#include "FSM/Estados_Sistema_Eco_Hidro.h"

#include "Mediador_Casa_Torre.h"
#include "Mediador_Bomba_Casa.h"
#include "Mediador_Reservatorio.h"
#include "Mediador_Bomba_12v.h"
#include "Mediador_Luz_Estacionamento.h"
#include "Mediador_Luz_Acesso_Reservatorio.h"
#include "Mediador_Voltagem.h"
#include "Mediador_Unidade_Potencia.h"
#include "Mediador_Potencia.h"
#include "Termico_Bomba_Casa/Mediador_Termico_Bomba_Casa.h"
#include "Giro_Preventivo_Bomba_Casa/Mediador_Giro_Preventivo_Bomba_Casa.h"
 

//====================================================================================
//                   VARIÁVEIS INTERNAS
//====================================================================================

//====================================================================================
//                   SETUP
//====================================================================================

void MEDIADOR_BOOT()
{
    debugSetupArquivo();
    debugSetupDivisao(COR::AMARELO, "MEDIADOR INICIALIZANDO", COR::MAGENTA, "=", 0);

    Mediador_Casa_Torre_Inicializa();

    Mediador_Bomba_Casa_Inicializa();

    Mediador_Reservatorio_Inicializa();

    Mediador_Bomba_12v_Inicializa();

    Mediador_Voltagem_Inicializa();

    Mediador_Luz_Estacionamento_Inicializa();

    Mediador_Luz_Acesso_Reservatorio_Inicializa();
    
    Mediador_Potencia_Inicializa();

    Mediador_Unidade_Potencia_Inicializa();

    Mediador_Termico_Bomba_Casa_Inicializa();

    Mediador_Giro_Preventivo_Bomba_Casa_Inicializa();

    //--------------------------------------------------------------------------------
    //  ESTADOS INICIAIS
    //--------------------------------------------------------------------------------
//não tem arquivos de Gerencia então carregam por aqui mesmo

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

    Mediador_Bomba_Casa_Atualiza();

    Mediador_Reservatorio_Atualiza();

    Mediador_Bomba_12v_Atualiza();

    Mediador_Voltagem_Atualiza();

    Mediador_Luz_Acesso_Reservatorio_Atualiza();

    Mediador_Luz_Estacionamento_Atualiza();

    Mediador_Potencia_Atualiza();

    Mediador_Unidade_Potencia_Atualiza();

    Mediador_Termico_Bomba_Casa_Atualiza();

    Mediador_Giro_Preventivo_Bomba_Casa_Atualiza();
}