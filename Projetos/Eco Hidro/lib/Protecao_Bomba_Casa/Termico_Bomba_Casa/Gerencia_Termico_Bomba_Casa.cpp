//====================================================================================
//                   DEPENDÊNCIAS
//====================================================================================

#include "Gerencia_Termico_Bomba_Casa.h"
#include "DEBUG_MODE.h"

#include "FSM/Estados_Sistema_Eco_Hidro.h"
#include "Controle_Termico_Bomba_Casa.h"

//====================================================================================
//                   VARIÁVEIS INTERNAS
//====================================================================================

//====================================================================================
//                   SETUP
//====================================================================================

void Gerencia_Termico_Bomba_Casa_Inicializa()
{
        debugSetupDivisao(COR::AMARELO, "GERENCIA TERMICO BOMBA CASA INICIALIZANDO", COR::MAGENTA, "=", 0);
        PROTECAO_TERMICA_BOMBA_CASA::definir(PROTECAO_TERMICA_BOMBA_CASA::SITUACAO::REPOUSO);
}

//====================================================================================
//                   LOOP
//====================================================================================

void Gerencia_Termico_Bomba_Casa_Atualiza()
{

    switch (PROTECAO_TERMICA_BOMBA_CASA::atual()) {

    //======================================================================
    case PROTECAO_TERMICA_BOMBA_CASA::SITUACAO::REPOUSO:
        //não faz nada
    break;
    
    //======================================================================
    case PROTECAO_TERMICA_BOMBA_CASA::SITUACAO::SUPERAQUECIMENTO:
        Controle_Termico_Bomba_Casa_Superaquecimento();
    break;

    //======================================================================
    case PROTECAO_TERMICA_BOMBA_CASA::SITUACAO::MONITORAMENTO_ATIVADO:
        Controle_Termico_Bomba_Casa_Monitoramento_Ativado();
    break;

    //======================================================================
    case PROTECAO_TERMICA_BOMBA_CASA::SITUACAO::MODO_SEGURANCA_PRIMEIRO_CICLO:
        Controle_Termico_Bomba_Casa_Modo_Seguranca_Primeiro_Ciclo();
    break;

    //======================================================================
    case PROTECAO_TERMICA_BOMBA_CASA::SITUACAO::MODO_SEGURANCA_SEGUNDO_CICLO:
        Controle_Termico_Bomba_Casa_Modo_Seguranca_Segundo_Ciclo();
    break;

    //======================================================================
    case PROTECAO_TERMICA_BOMBA_CASA::SITUACAO::FINALIZADO:
        Controle_Termico_Bomba_Casa_Finalizado();
    break;

    default:
        PROTECAO_TERMICA_BOMBA_CASA::definir(PROTECAO_TERMICA_BOMBA_CASA::SITUACAO::REPOUSO);
    break;

    }
}
