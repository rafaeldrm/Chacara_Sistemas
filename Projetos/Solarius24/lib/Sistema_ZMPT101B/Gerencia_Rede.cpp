//====================================================================================
//                   DEPENDÊNCIAS
//====================================================================================

#include "Gerencia_Rede.h"
#include "DEBUG_MODE.h"
#include "Controle_Rede.h"

//====================================================================================
//                   SETUP
//====================================================================================

void Gerencia_Rede_Inicializa() 
{

    debugSetupArquivo();
    debugSetupTexto("GERÊNCIA REDE", NEGRITO | AMARELO | LINHA);
    debugVariavel("ESTADO REDE :: ", ESTADO::REDE::texto(ESTADO::REDE::atual()), NEGRITO, VERDE, LARANJA);

}

//====================================================================================
//                   LOOP
//====================================================================================

void Gerencia_Rede_Atualiza() {

    ESTADO::REDE::STATUS estadoAtual = ESTADO::REDE::atual();
    ESTADO::REDE::STATUS EstadoRede = estadoAtual;

    // 1. Máquina de Estados da Rede Elétrica
    switch (ESTADO::REDE::atual())
    {

    case ESTADO::REDE::STATUS::DORMINDO:
        // Não faz nada, o monitoramento está inativo.
        break;

    case ESTADO::REDE::STATUS::VERIFICANDO_5S:
         EstadoRede = Controle_Rede_Logica_Verificando_5s();
        break;

           case ESTADO::REDE::STATUS::AUSENTE:
            EstadoRede = Controle_Rede_Logica_Ausente();
            break;

        case ESTADO::REDE::STATUS::AGUARDANDO_5MIN:
            EstadoRede = Controle_Rede_Logica_Aguardando_5min();
            break;

        case ESTADO::REDE::STATUS::INSTAVEL_15MIN:
            EstadoRede = Controle_Rede_Logica_Instavel_15min();
            break;

        case ESTADO::REDE::STATUS::MONITORANDO_FONTE:
            EstadoRede = Controle_Rede_Logica_Monitorando_Fonte();
            break;

        case ESTADO::REDE::STATUS::CARGA_FINALIZADA:
            EstadoRede = Controle_Rede_Logica_Carga_Finalizada();
            break;
        
        case ESTADO::REDE::STATUS::CALAMIDADE:
            EstadoRede = Controle_Rede_Logica_Calamidade();
            break;

    default:
        debugErro("ERRO CRÍTICO: Estado desconhecido! Forçando Normal.");
         EstadoRede = ESTADO::REDE::STATUS::DORMINDO; //ver EstadoSistema.cpp
        break;
    }

     // Só muda o estado se realmente mudou
    if (EstadoRede != estadoAtual) {
      
      debugVariavel("REDE: Mudou de ", ((uint8_t)estadoAtual), NEGRITO, VERDE, LARANJA);
      ESTADO::REDE::definir(EstadoRede);
      debugVariavel("REDE: Mudou para ", ((uint8_t)EstadoRede), NEGRITO, VERDE, LARANJA);

    }
}