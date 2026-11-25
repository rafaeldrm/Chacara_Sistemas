#pragma once
#include <Arduino.h>

//----------------------------  ORDEM DE HIERARQUIA ---------------------------
    
    //====================================================================================
    //                   ESTADO DA BATERIA (INA3221)
    //====================================================================================
   
namespace BATERIA
{
    enum class ESTADO : uint8_t
    { 
        NORMAL, 
        OBSERVA, 
        CALAMIDADE_INICIO, 
        CALAMIDADE_DISJUNTOR_DESLIGADO, 
        CALAMIDADE_COMPLETO, 
        TENTATIVA_RECUPERACAO
    };

        void definir(ESTADO novo); 
        ESTADO atual();
        const __FlashStringHelper* texto(ESTADO s);
    }

    //====================================================================================
    //                   ESTADO DE OPERAÇÃO  (SISTEMA RESIDENCIAL)
    //====================================================================================

namespace URGENCIA
{
    enum class ESTADO : uint8_t
    { 
        NORMAL = 0, 
        AUTOMATICO,
        CALAMIDADE 
    };

        void definir(ESTADO novo);
        ESTADO atual();
        const __FlashStringHelper* texto(ESTADO m);
    }

    //====================================================================================
    //                   ESTADO DA VOLTAGEM (INA226)
    //====================================================================================
namespace VOLTAGEM 
{
    enum class ESTADO : uint8_t
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

        void definir(ESTADO novo);
        ESTADO atual();
        const __FlashStringHelper* texto(ESTADO n);
    }

    //====================================================================================
    //                   ESTADO DA REDE (ZMPT101B)
    //====================================================================================
   
namespace REDE 
{
    enum class ESTADO : uint8_t 
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

        void definir(ESTADO novo);
        ESTADO atual();
        const __FlashStringHelper* texto(ESTADO s);
    }
