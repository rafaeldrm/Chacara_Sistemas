//====================================================================================
//                   DEPENDÊNCIAS
//====================================================================================

#include "Escalonador.h"
#include "DEBUG_MODE.h"

#include "FSM/Estados_Sistema_Eco_Hidro.h"

//====================================================================================
//                   VARIÁVEIS INTERNAS
//====================================================================================

    namespace ESCALONADOR
{
    //==================================================
    // SEQUÊNCIA DE AUTORIZAÇÃO
    //==================================================

    enum class FSM : uint8_t
    {
        CASA_TORRE,
        BOMBA_CASA,
        RESERVATORIO,
        BOMBA_12V,
        SISTEMA_ECO_HIDRO,
        LUZ_ESTACIONAMENTO,
        LUZ_ACESSO,
        VOLTAGEM,
        DESLIGAMENTO_BOMBA,
        PORTAO,
        UNIDADE_POTENCIA,
        POTENCIA,
        TRANCO_BOMBA_CASA,
        PROTECAO_TERMICA_BOMBA_CASA,
        GIRO_PREVENTIVO_BOMBA_CASA,

        TOTAL
    };


    // Primeira FSM autorizada após o boot
    static FSM FSM_Autorizada = FSM::CASA_TORRE;


    //==================================================
    // EXECUTA UMA ÚNICA FSM
    //==================================================

    static void Executa_FSM()
    {
        switch (FSM_Autorizada)
        {
            case FSM::CASA_TORRE:
                CASA_TORRE::atualizarCiclo();
            break;

            case FSM::BOMBA_CASA:
                BOMBA_CASA::atualizarCiclo();
            break;

            case FSM::RESERVATORIO:
                RESERVATORIO::atualizarCiclo();
            break;

            case FSM::BOMBA_12V:
                BOMBA_12V::atualizarCiclo();
            break;

            case FSM::SISTEMA_ECO_HIDRO:
                SISTEMA_ECO_HIDRO::atualizarCiclo();
            break;
 
            case FSM::LUZ_ESTACIONAMENTO:
                LUZ_ESTACIONAMENTO::atualizarCiclo();
            break;

            case FSM::LUZ_ACESSO:
                LUZ_ACESSO::atualizarCiclo();
            break;

            case FSM::VOLTAGEM:
                VOLTAGEM::atualizarCiclo();
            break;

            case FSM::DESLIGAMENTO_BOMBA:
                DESLIGAMENTO_BOMBA::atualizarCiclo();
            break;
            
            case FSM::PORTAO:
                PORTAO::atualizarCiclo();
            break;

            case FSM::UNIDADE_POTENCIA:
                UNIDADE_POTENCIA::atualizarCiclo();
            break;
                
            case FSM::POTENCIA:
                POTENCIA::atualizarCiclo();
            break;

            case FSM::TRANCO_BOMBA_CASA:
                TRANCO_BOMBA_CASA::atualizarCiclo();
            break;

            case FSM::PROTECAO_TERMICA_BOMBA_CASA:
                PROTECAO_TERMICA_BOMBA_CASA::atualizarCiclo();
            break;

            case FSM::GIRO_PREVENTIVO_BOMBA_CASA:
                GIRO_PREVENTIVO_BOMBA_CASA::atualizarCiclo();
            break;

            
            default:
                break;
        }
    }


    //==================================================
    // AVANÇA PARA A PRÓXIMA FSM
    //==================================================

    static void Proxima_FSM()
    {
        uint8_t proxima =
            static_cast<uint8_t>(FSM_Autorizada) + 1;

        if (proxima >= static_cast<uint8_t>(FSM::TOTAL))
        {
            proxima = 0;
        }

        FSM_Autorizada = static_cast<FSM>(proxima);
    }
}

//====================================================================================
//                   SETUP
//====================================================================================

void ESCALONADOR_BOOT()
{
        debugSetupDivisao(COR::AMARELO, "ESCALONADOR INICIALIZANDO", COR::MAGENTA, "=", 0);
}

//====================================================================================
//                   LOOP
//====================================================================================

void ESCALONADOR_LOOP()
{
    ESCALONADOR::Executa_FSM();

    // Avança mesmo quando não houve transição.
    ESCALONADOR::Proxima_FSM();
}
