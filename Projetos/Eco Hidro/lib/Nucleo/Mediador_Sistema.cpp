//====================================================================================
//                   DEPENDÊNCIAS
//====================================================================================

#include "Mediador_Sistema.h"
#include "DEBUG_MODE.h"

#include "Estados_Sistema_Eco_Hidro.h"
#include "Estados_Eventos_Eco_Hidro.h"

#include "Mediador_Casa_Torre.h"
#include "Mediador_Reservatorio.h"
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

    Mediador_Unidade_Potencia_Inicializa();

    //--------------------------------------------------------------------------------
    //  ESTADOS INICIAIS
    //--------------------------------------------------------------------------------

    CASA_TORRE::definir(CASA_TORRE::ETAPAS::NORMAL);
    BOMBA_CASA::definir(BOMBA_CASA::ESTAGIOS::REPOUSO);
    RESERVATORIO::definir(RESERVATORIO::TRANSFERENCIA::NORMAL);
    BOMBA_12V::definir(BOMBA_12V::ESTAGIOS::REPOUSO);
    SISTEMA_ECO_HIDRO::definir(SISTEMA_ECO_HIDRO::MODO::NORMAL);
    LUZ_ESTACIONAMENTO::definir(LUZ_ESTACIONAMENTO::SITUACAO::DESLIGADO);
    LUZ_ACESSO::definir(LUZ_ACESSO::SITUACAO::TEMPORIZADO);
    VOLTAGEM::definir(VOLTAGEM::LEITURA::NORMAL);
    DESLIGAMENTO_BOMBA::definir(DESLIGAMENTO_BOMBA::MOTIVO::NENHUM);
    PORTAO_ELETRONICO::definir(PORTAO_ELETRONICO::ESTAGIOS::REPOUSO);
    UNIDADE_POTENCIA::definir(UNIDADE_POTENCIA::ESTAGIOS::REPOUSO);
    POTENCIA::definir(POTENCIA::SELECIONADO::REPOUSO);

    //--------------------------------------------------------------------------------
    //  EVENTOS LoRa
    //--------------------------------------------------------------------------------

    ORDENS_DRIMEL_SYSTEM::CASA_TORRE::definir(ORDENS_DRIMEL_SYSTEM::CASA_TORRE::ORDEM::NENHUM);
    ORDENS_DRIMEL_SYSTEM::LUZ_ACESSO::definir(ORDENS_DRIMEL_SYSTEM::LUZ_ACESSO::ORDEM::NENHUM);
    ORDENS_DRIMEL_SYSTEM::LUZ_ESTACIONAMENTO::definir(ORDENS_DRIMEL_SYSTEM::LUZ_ESTACIONAMENTO::ORDEM::NENHUM);
}

//====================================================================================
//                   LOOP
//====================================================================================

void MEDIADOR_LOOP()
{
    Mediador_Casa_Torre_Atualiza();

    Mediador_Reservatorio_Atualiza();

    // Voltagem em off por enquanto (era Inicializa() no loop, agora é Atualiza() comentado).
    // Mediador_Voltagem_Atualiza();

    Mediador_Unidade_Potencia_Atualiza();   // SEMPRE por último: lê os pedidos feitos acima.
}