#pragma once
#include <Arduino.h>

//----------------------------  ORDEM DE HIERARQUIA ---------------------------
    
    //====================================================================================
    //                   ESTADO DA BATERIA (INA3221)
    //====================================================================================
   
namespace BATERIA
{
    enum class SITUACAO : uint8_t
    { 
        NORMAL, 
        OBSERVA, 
        CALAMIDADE_INICIO, 
        CALAMIDADE_DISJUNTOR_DESLIGADO, 
        CALAMIDADE_COMPLETO, 
        TENTATIVA_RECUPERACAO
    };

        void definir(SITUACAO novo); 
        SITUACAO atual();
        const __FlashStringHelper* texto(SITUACAO s);
    }

    //====================================================================================
    //                   MODO DE OPERAÇÃO  (SISTEMA RESIDENCIAL)
    //====================================================================================

namespace URGENCIA
{
    enum class MODO : uint8_t
    { 
        NORMAL = 0, 
        AUTOMATICO,
        CALAMIDADE 
    };

        void definir(MODO novo);
        MODO atual();
        const __FlashStringHelper* texto(MODO m);
    }

    //====================================================================================
    //                   ESTADO DA VOLTAGEM (INA226)
    //====================================================================================
namespace VOLTAGEM 
{
    enum class NIVEL : uint8_t
    {
        NORMAL = 0,
        AUXILIAR,
        AUTOMATICO,
        FIM_AUTOMATICO,
        EMERGENCIAL, 
        CRITICO,
        RECUPERANDO_CARGA,
        RECUPERADO, 
        CALAMIDADE_INICIO, 
        CALAMIDADE_COMPLETO, 
        TENTATIVA_RECUPERACAO       
    };

        void definir(NIVEL novo);
        NIVEL atual();
        const __FlashStringHelper* texto(NIVEL n);
    }

    //====================================================================================
    //                   ESTADO DA REDE (ZMPT101B)
    //====================================================================================
   
namespace REDE 
{
    enum class STATUS : uint8_t 
    {
        DORMINDO = 0,
        VERIFICANDO_5S,
        AUSENTE, 
        AGUARDANDO_5MIN,
        INSTAVEL_15MIN,
        MONITORANDO_FONTE,
        CARGA_FINALIZADA,
        CALAMIDADE
    };

        void definir(STATUS novo);
        STATUS atual();
        const __FlashStringHelper* texto(STATUS s);
    }
