#include "Estados_Sistema_Solarius24.h"
#include "DEBUG_MODE.h"

//====================================================================================
//                   VARIÁVEIS INTERNAS (ESTÁTICAS)
//====================================================================================

struct estadoSistema {
    BATERIA::SITUACAO bateria;
    URGENCIA::MODO urgencia;
    VOLTAGEM::NIVEL voltagem;
    REDE::STATUS rede;
};

static estadoSistema estado = {
    BATERIA::SITUACAO::NORMAL,
    URGENCIA::MODO::NORMAL,
    VOLTAGEM::NIVEL::NORMAL,
    REDE::STATUS::DORMINDO,
};

//====================================================================================
//             BATERIA
//====================================================================================

namespace BATERIA 
{

    void definir(SITUACAO novo)
    {
        if (estado.bateria == novo) return;
    estado.bateria = novo;
    debugVariavel("BATERIA : ", texto(novo), NENHUM, VERDE, AMARELO);
}

SITUACAO atual() 
{
    return estado.bateria;
}

    const __FlashStringHelper* texto(SITUACAO a)
    {
        switch(a)
        {
            case SITUACAO::NORMAL:                         return F("NORMAL");
            case SITUACAO::OBSERVA:                        return F("OBSERVA");
            case SITUACAO::CALAMIDADE_INICIO:              return F("CALAMIDADE_INICIO");
            case SITUACAO::CALAMIDADE_DISJUNTOR_DESLIGADO: return F("CALAMIDADE_DISJUNTOR_DESLIGADO");
            case SITUACAO::CALAMIDADE_COMPLETO:            return F("CALAMIDADE_COMPLETO");
            case SITUACAO::TENTATIVA_RECUPERACAO:          return F("TENTATIVA_RECUPERACAO");
            
            default:                                       return F("DESCONHECIDO");
        }
    }
}

//====================================================================================
//                   URGÊNCIA
//====================================================================================

namespace URGENCIA {

    void definir(MODO novo) {
        if (estado.urgencia == novo) return;

        estado.urgencia = novo;
         debugVariavel("Modo Urgência : ", texto(novo), NENHUM, VERDE, AMARELO);
    }

    MODO atual() 
    {
        return estado.urgencia;
    }

    const __FlashStringHelper* texto(MODO b) 
    {
        switch(b)
        {
            case MODO::NORMAL:     return F("NORMAL");
            case MODO::AUTOMATICO: return F("AUTOMATICO");
            case MODO::CALAMIDADE: return F("CALAMIDADE");
            
            default:               return F("DESCONHECIDO");
        }
    }
}

//====================================================================================
//                   VOLTAGEM
//====================================================================================

namespace VOLTAGEM
{

    void definir(NIVEL novo) 
    {
            if (estado.voltagem == novo) return;
        estado.voltagem = novo;
         debugVariavel("Estado Carga : ", texto(novo), NENHUM, VERDE, AMARELO);
    }

    NIVEL atual() 
    {
        return estado.voltagem;
    }

    const __FlashStringHelper* texto(NIVEL c) 
    {
        switch(c) 
        {
            case NIVEL::NORMAL:                return F("NORMAL");
            case NIVEL::AUXILIAR:              return F("AUXILIAR");
            case NIVEL::AUTOMATICO:            return F("AUTOMATICO");
            case NIVEL::FIM_AUTOMATICO:        return F("FIM_AUTOMATICO");
            case NIVEL::EMERGENCIAL:           return F("EMERGENCIAL");
            case NIVEL::CRITICO:               return F("CRITICO");
            case NIVEL::RECUPERANDO_CARGA:     return F("RECUPERANDO_CARGA");
            case NIVEL::RECUPERADO:            return F("RECUPERADO");
            case NIVEL::CALAMIDADE_INICIO:     return F("CALAMIDADE_INICIO");
            case NIVEL::CALAMIDADE_COMPLETO:   return F("CALAMIDADE_COMPLETO");
            case NIVEL::TENTATIVA_RECUPERACAO: return F("TENTATIVA_RECUPERACAO");
            
            default:                           return F("DESCONHECIDO");
        }
    }
}

//====================================================================================
//                   REDE
//====================================================================================

namespace REDE 
{

    void definir(STATUS novo) 
    {
                  if (estado.rede == novo) return;
        estado.rede = novo;
         debugVariavel("Estado Rede : ", texto(novo), NENHUM, VERDE, AMARELO);
    }

    STATUS atual() 
    {
        return estado.rede;
    }

    const __FlashStringHelper* texto(STATUS d) 
    {
        switch(d)
        {
            case STATUS::DORMINDO:          return F("DORMINDO");
            case STATUS::VERIFICANDO_5S:    return F("VERIFICANDO_5S");
            case STATUS::AUSENTE:           return F("AUSENTE");
            case STATUS::AGUARDANDO_5MIN:   return F("AGUARDANDO_5MIN");
            case STATUS::INSTAVEL_15MIN:    return F("INSTAVEL_15MIN");
            case STATUS::MONITORANDO_FONTE: return F("MONITORANDO_FONTE");
            case STATUS::CARGA_FINALIZADA:  return F("CARGA_FINALIZADA");
            case STATUS::CALAMIDADE:        return F("CALAMIDADE");
            
            default:                        return F("DESCONHECIDO");
        }
    }
}