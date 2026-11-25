#include "Estados_Sistema_Solarius24.h"
#include "DEBUG_MODE.h"

//====================================================================================
//                   VARIÁVEIS INTERNAS (ESTÁTICAS)
//====================================================================================

struct estadoSistema {
    BATERIA::ESTADO bateria;
    URGENCIA::ESTADO urgencia;
    VOLTAGEM::ESTADO voltagem;
    REDE::ESTADO rede;
};

static estadoSistema estado = {
    BATERIA::ESTADO::NORMAL,
    URGENCIA::ESTADO::NORMAL,
    VOLTAGEM::ESTADO::NORMAL,
    REDE::ESTADO::DORMINDO,
};

//====================================================================================
//             BATERIA
//====================================================================================

namespace BATERIA 
{

    void definir(ESTADO novo)
    {
        if (estado.bateria == novo) return;
    estado.bateria = novo;
    debugVariavel("BATERIA : ", texto(novo), NENHUM, VERDE, AMARELO);
}

ESTADO atual() 
{
    return estado.bateria;
}

    const __FlashStringHelper* texto(ESTADO a)
    {
        switch(a)
        {
            case ESTADO::NORMAL:                         return F("NORMAL");
            case ESTADO::OBSERVA:                        return F("OBSERVA");
            case ESTADO::CALAMIDADE_INICIO:              return F("CALAMIDADE_INICIO");
            case ESTADO::CALAMIDADE_DISJUNTOR_DESLIGADO: return F("CALAMIDADE_DISJUNTOR_DESLIGADO");
            case ESTADO::CALAMIDADE_COMPLETO:            return F("CALAMIDADE_COMPLETO");
            case ESTADO::TENTATIVA_RECUPERACAO:          return F("TENTATIVA_RECUPERACAO");
            
            default:                                       return F("DESCONHECIDO");
        }
    }
}

//====================================================================================
//                   URGÊNCIA
//====================================================================================

namespace URGENCIA {

    void definir(ESTADO novo) {
        if (estado.urgencia == novo) return;

        estado.urgencia = novo;
         debugVariavel("Modo Urgência : ", texto(novo), NENHUM, VERDE, AMARELO);
    }

    ESTADO atual() 
    {
        return estado.urgencia;
    }

    const __FlashStringHelper* texto(ESTADO b) 
    {
        switch(b)
        {
            case ESTADO::NORMAL:     return F("NORMAL");
            case ESTADO::AUTOMATICO: return F("AUTOMATICO");
            case ESTADO::CALAMIDADE: return F("CALAMIDADE");
            
            default:               return F("DESCONHECIDO");
        }
    }
}

//====================================================================================
//                   VOLTAGEM
//====================================================================================

namespace VOLTAGEM
{

    void definir(ESTADO novo) 
    {
            if (estado.voltagem == novo) return;
        estado.voltagem = novo;
         debugVariavel("Estado Carga : ", texto(novo), NENHUM, VERDE, AMARELO);
    }

    ESTADO atual() 
    {
        return estado.voltagem;
    }

    const __FlashStringHelper* texto(ESTADO c) 
    {
        switch(c) 
        {
            case ESTADO::NORMAL:                return F("NORMAL");
            case ESTADO::AUXILIAR:              return F("AUXILIAR");
            case ESTADO::AUTOMATICO:            return F("AUTOMATICO");
            case ESTADO::FIM_AUTOMATICO:        return F("FIM_AUTOMATICO");
            case ESTADO::EMERGENCIAL:           return F("EMERGENCIAL");
            case ESTADO::CRITICO:               return F("CRITICO");
            case ESTADO::RECUPERANDO_CARGA:     return F("RECUPERANDO_CARGA");
            case ESTADO::RECUPERADO:            return F("RECUPERADO");
            case ESTADO::CALAMIDADE_INICIO:     return F("CALAMIDADE_INICIO");
            case ESTADO::CALAMIDADE_COMPLETO:   return F("CALAMIDADE_COMPLETO");
            case ESTADO::TENTATIVA_RECUPERACAO: return F("TENTATIVA_RECUPERACAO");
            
            default:                           return F("DESCONHECIDO");
        }
    }
}

//====================================================================================
//                   REDE
//====================================================================================

namespace REDE 
{

    void definir(ESTADO novo) 
    {
                  if (estado.rede == novo) return;
        estado.rede = novo;
         debugVariavel("Estado Rede : ", texto(novo), NENHUM, VERDE, AMARELO);
    }

    ESTADO atual() 
    {
        return estado.rede;
    }

    const __FlashStringHelper* texto(ESTADO d) 
    {
        switch(d)
        {
            case ESTADO::DORMINDO:          return F("DORMINDO");
            case ESTADO::VERIFICANDO_5S:    return F("VERIFICANDO_5S");
            case ESTADO::AUSENTE:           return F("AUSENTE");
            case ESTADO::AGUARDANDO_5MIN:   return F("AGUARDANDO_5MIN");
            case ESTADO::INSTAVEL_15MIN:    return F("INSTAVEL_15MIN");
            case ESTADO::MONITORANDO_FONTE: return F("MONITORANDO_FONTE");
            case ESTADO::CARGA_FINALIZADA:  return F("CARGA_FINALIZADA");
            case ESTADO::CALAMIDADE:        return F("CALAMIDADE");
            
            default:                        return F("DESCONHECIDO");
        }
    }
}