//==================================================================================== 
//                   DEPENDÊNCIAS 
//==================================================================================== 

#include "Estado_Sistema_Lavanderia.h" 
#include "DEBUG_MODE.h" 

//==================================================================================== 
//                   VARIÁVEIS INTERNAS (ESTÁTICAS) 
//==================================================================================== 

constexpr uint8_t QTD_NICHOS = static_cast<uint8_t>(NICHO_CATEGORIA::TOTAL_CATEGORIAS);

struct EstadosSistema 
{ 
    NICHO::ETAPA nicho[QTD_NICHOS];
}; 

static EstadosSistema estado = {}; // Inicializa todos os nichos com ETAPA::NENHUM

//==================================================================================== 
//                   IMPLEMENTAÇÃO NICHO 
//==================================================================================== 

namespace NICHO 
{ 
    void definir(uint8_t indice, ETAPA novo)
    {
        if (indice >= QTD_NICHOS) return;
        if (estado.nicho[indice] == novo) return;

        estado.nicho[indice] = novo;

        debugVariavel("ETAPA nicho : ", texto(novo), NENHUM, VERDE, AMARELO);
    }

    ETAPA atual(uint8_t indice)
    {
        if (indice >= QTD_NICHOS) return ETAPA::NENHUM;
        return estado.nicho[indice];
    }

    const __FlashStringHelper* texto(ETAPA a) 
    { 
        switch(a) 
        { 
            case ETAPA::NENHUM:             return F("NENHUM");
            case ETAPA::ACESSANDO:          return F("ACESSANDO");
            case ETAPA::CONTABILIZANDO:     return F("CONTABILIZANDO");
            case ETAPA::PRONTO_PARA_LAVAR:  return F("PRONTO_PARA_LAVAR");
            case ETAPA::RESETANDO:          return F("RESETANDO");
            case ETAPA::FINALIZADO:         return F("FINALIZADO");
            default:                        return F("DESCONHECIDO"); 
        } 
    } 
}