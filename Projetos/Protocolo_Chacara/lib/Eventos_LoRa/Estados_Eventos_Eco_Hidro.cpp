//====================================================================================
//                   DEPENDÊNCIAS
//====================================================================================

#include "Estados_Eventos_Eco_Hidro.h"
#include "DEBUG_MODE.h"

//====================================================================================                              
//                   VARIÁVEIS INTERNAS (ESTÁTICAS)                              
//====================================================================================                              
                              
struct estadosEventosEcoHidro                               
{                                                    
    ORDENS_DRIMEL_SYSTEM::CASA_TORRE::ORDEM ordemCasaTorre;
    ORDENS_DRIMEL_SYSTEM::LUZ_ACESSO::ORDEM ordemLuzAcesso;
        ORDENS_DRIMEL_SYSTEM::LUZ_ESTACIONAMENTO::ORDEM ordemLuzEstacionamento;                     
};                              
                              
static estadosEventosEcoHidro estado =                               
{                               
    ORDENS_DRIMEL_SYSTEM::CASA_TORRE::ORDEM::NENHUM,
    ORDENS_DRIMEL_SYSTEM::LUZ_ACESSO::ORDEM::NENHUM,
    ORDENS_DRIMEL_SYSTEM::LUZ_ESTACIONAMENTO::ORDEM::NENHUM,
};                              
                              
//====================================================================================                              
//                   IMPLEMENTAÇÃO DOS NAMESPACES                              
//====================================================================================                              
   
//====================================================================================   
//                   ORDENS_DRIMEL_SYSTEM   
//====================================================================================   
   
namespace ORDENS_DRIMEL_SYSTEM   
{   
    namespace CASA_TORRE   
    {   
        void definir(ORDEM novo)   
        {   
            if (estado.ordemCasaTorre == novo) return;   
            estado.ordemCasaTorre = novo;   
            debugVariavel("ORDEM ordemCasaTorre : ", texto(novo), LINHA, VERDE, AMARELO);   
        }   
   
        ORDEM atual()   
        {   
            return estado.ordemCasaTorre;   
        }   
   
        const __FlashStringHelper* texto(ORDEM a)   
        {   
            switch(a)   
            {   
                case ORDEM::NENHUM:                                 return F("NENHUM");   
                case ORDEM::CICLO_COMPLETO:                         return F("CICLO_COMPLETO");   
                case ORDEM::MEIO_CICLO:                             return F("MEIO_CICLO");   
                case ORDEM::BLOQUEIA:                               return F("BLOQUEIA");   
                case ORDEM::DESLIGA:                                return F("DESLIGA");   
   
                default:                return F("DESCONHECIDO");   
            }   
        }   
    }   

     namespace LUZ_ACESSO   
    {   
        void definir(ORDEM novo)   
        {   
            if (estado.ordemLuzAcesso == novo) return;   
            estado.ordemLuzAcesso = novo;   
            debugVariavel("ORDEM ordemLuzAcesso : ", texto(novo), LINHA, VERDE, AMARELO);   
        }   
   
        ORDEM atual()   
        {   
            return estado.ordemLuzAcesso;   
        }   
   
        const __FlashStringHelper* texto(ORDEM a)   
        {   
            switch(a)   
            {   
                case ORDEM::NENHUM:                                 return F("NENHUM");   
                case ORDEM::TEMPORIZACAO:                           return F("TEMPORIZACAO");
                case ORDEM::ACIONADO:                                return F("ACIONADO");          
                case ORDEM::FORCADO:                                return F("FORCADO");   
                case ORDEM::DESLIGA:                                return F("DESLIGA");   
   
                default:                return F("DESCONHECIDO");   
            }   
        }   
    }   

     namespace LUZ_ESTACIONAMENTO   
    {   
        void definir(ORDEM novo)   
        {   
            if (estado.ordemLuzEstacionamento == novo) return;   
            estado.ordemLuzEstacionamento = novo;   
            debugVariavel("ORDEM ordemLuzEstacionamento : ", texto(novo), LINHA, VERDE, AMARELO);   
        }   
   
        ORDEM atual()   
        {   
            return estado.ordemLuzEstacionamento;   
        }   
   
        const __FlashStringHelper* texto(ORDEM a)   
        {   
            switch(a)   
            {   
                case ORDEM::NENHUM:                                 return F("NENHUM");   
                case ORDEM::TEMPORIZACAO:                           return F("TEMPORIZACAO");
                case ORDEM::ACIONADO:                               return F("ACIONADO");   
                case ORDEM::FORCADO:                                return F("FORCADO");   
                case ORDEM::DESLIGA:                                return F("DESLIGA");   
   
                default:                return F("DESCONHECIDO");   
            }   
        }   
    }   
}   