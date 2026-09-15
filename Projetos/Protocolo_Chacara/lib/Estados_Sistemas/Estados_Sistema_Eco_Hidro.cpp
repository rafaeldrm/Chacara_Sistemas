//====================================================================================                           
//                   DEPENDÊNCIAS                           
//====================================================================================                           
                                                    
#include "Estados_Sistema_Eco_Hidro.h"                           
#include "DEBUG_MODE.h"                           
                           
//====================================================================================                           
//                   VARIÁVEIS INTERNAS (ESTÁTICAS)                           
//====================================================================================                           
                           
struct EstadosSistemaEcoHidro                            
{                           
                           
        CASA_TORRE::ETAPAS casaTorre; // [VAR::CASA_TORRE]
        BOMBA_CASA::ESTAGIOS bombaCasa; // [VAR::BOMBA_CASA]  
        RESERVATORIO::TRANSFERENCIA transfereReserva; // [VAR::RESERVATORIO]                        
        BOMBA_12V::ESTAGIOS bombaReserva; // [VAR::BOMBA_12V]     
        SISTEMA_ECO_HIDRO::MODO sistemaEcoHidroModo; // [VAR::SISTEMA_ECO_HIDRO]             
        LUZ_ESTACIONAMENTO::SITUACAO luzEstacionamento; // [VAR::LUZ_ESTACIONAMENTO]                     
        LUZ_ACESSO::SITUACAO luzReservatorio; // [VAR::LUZ_ACESSO]                
        VOLTAGEM::LEITURA bateria; // [VAR::VOLTAGEM]                   
        DESLIGAMENTO_BOMBA::MOTIVO motivoBomba; // [VAR::DESLIGAMENTO_BOMBA]      
        PORTAO_ELETRONICO::SITUACAO portao; // [VAR::PORTAO_ELETRONICO]                                 
        UNIDADE_POTENCIA::ESTAGIOS unidadePotencia; // [VAR::UNIDADE_POTENCIA]
        POTENCIA::SELECIONADO potenciaSelecionado; // [VAR::POTENCIA]
        ORDENS_DRIMEL_SYSTEM::CASA_TORRE::ORDEM ordemCasaTorre; // [VAR::ORDENS_DRIMEL_SYSTEM::CASA_TORRE]
                           
    // [STRUCT::BAT]
                           
};                           
                           
static EstadosSistemaEcoHidro estado =                            
{                           
    CASA_TORRE::ETAPAS::NORMAL, // [INIT::CASA_TORRE]
    BOMBA_CASA::ESTAGIOS::REPOUSO, // [INIT::BOMBA_CASA]  
    RESERVATORIO::TRANSFERENCIA::NORMAL, // [INIT::RESERVATORIO]                        
    BOMBA_12V::ESTAGIOS::REPOUSO, // [INIT::BOMBA_12V]     
    SISTEMA_ECO_HIDRO::MODO::NORMAL, // [INIT::SISTEMA_ECO_HIDRO]             
    LUZ_ESTACIONAMENTO::SITUACAO::DESLIGADO, // [INIT::LUZ_ESTACIONAMENTO]                     
    LUZ_ACESSO::SITUACAO::DESLIGADO, // [INIT::LUZ_ACESSO]                
    VOLTAGEM::LEITURA::NORMAL, // [INIT::VOLTAGEM]                   
    DESLIGAMENTO_BOMBA::MOTIVO::NENHUM, // [INIT::DESLIGAMENTO_BOMBA]      
    PORTAO_ELETRONICO::SITUACAO::REPOUSO, // [INIT::PORTAO_ELETRONICO]             
    UNIDADE_POTENCIA::ESTAGIOS::REPOUSO,// [INIT::UNIDADE_POTENCIA]
    POTENCIA::SELECIONADO::REPOUSO, // [INIT::POTENCIA]
    ORDENS_DRIMEL_SYSTEM::CASA_TORRE::ORDEM::NENHUM// [STATIC::VIRGULA] //VAR::ORDENS_DRIMEL_SYSTEM::CASA_TORRE]  
                           
    // [STATIC::BAT]
};                           
                           
//====================================================================================                           
//                   IMPLEMENTAÇÃO DOS NAMESPACES                           
//====================================================================================                           
                        
// [INICIO::BAT::CASA_TORRE] 
 
//==================================================================================== 
//                   CASA_TORRE 
//==================================================================================== 
 
namespace CASA_TORRE 
{ 
    void definir(ETAPAS novo)  
    { 
        if (estado.casaTorre == novo) return; 
        estado.casaTorre = novo; 
        debugVariavel("ETAPAS casaTorre : ", texto(novo), NENHUM, VERDE, AMARELO); 
    } 
 
    ETAPAS atual() 
    { 
         return estado.casaTorre;  
    } 
 
    const __FlashStringHelper* texto(ETAPAS a) 
    { 
        switch(a) 
        { 
            case ETAPAS::NORMAL:                                      return F("NORMAL");
            case ETAPAS::PRIMEIRO_CICLO_ENCHIMENTO_CAIXA:             return F("PRIMEIRO_CICLO_ENCHIMENTO_CAIXA");
            case ETAPAS::AGUARDANDO_TRANSFERENCIA_GRAVIDADE:          return F("AGUARDANDO_TRANSFERENCIA_GRAVIDADE");
            case ETAPAS::SEGUNDO_CICLO_ENCHIMENTO_CAIXA:              return F("SEGUNDO_CICLO_ENCHIMENTO_CAIXA");
            case ETAPAS::FINALIZADO:                                  return F("FINALIZADO");
 
            default:                return F("DESCONHECIDO"); 
        } 
    } 
} 
  
// [FIM::BAT::CASA_TORRE] 
   
// [INICIO::BAT::BOMBA_CASA]   
   
//====================================================================================   
//                   BOMBA_CASA   
//====================================================================================   
   
namespace BOMBA_CASA   
{   
    void definir(ESTAGIOS novo)    
    {   
        if (estado.bombaCasa == novo) return;   
        estado.bombaCasa = novo;   
        debugVariavel("ESTAGIOS bombaCasa : ", texto(novo), NENHUM, VERDE, AMARELO);   
    }   
   
    ESTAGIOS atual()   
    {   
         return estado.bombaCasa;    
    }   
   
    const __FlashStringHelper* texto(ESTAGIOS a)   
    {   
        switch(a)   
        {   
            case ESTAGIOS::REPOUSO:                                   return F("REPOUSO");  
            case ESTAGIOS::LIBERADO:                                  return F("LIBERADO");  
            case ESTAGIOS::AGUARDA_LIBERACAO:                         return F("AGUARDA_LIBERACAO");  
            case ESTAGIOS::LIGANDO_BOMBA:                             return F("LIGANDO_BOMBA");  
            case ESTAGIOS::BOMBA_LIGADA:                              return F("BOMBA_LIGADA");  
            case ESTAGIOS::DESLIGANDO_BOMBA:                          return F("DESLIGANDO_BOMBA");    
   
            default:                return F("DESCONHECIDO");   
        }   
    }   
}   
    
// [FIM::BAT::BOMBA_CASA]   
           
// [INICIO::BAT::RESERVATORIO]                        
                        
//====================================================================================                        
//                   RESERVATORIO                        
//====================================================================================                        
                        
namespace RESERVATORIO                        
{                        
    void definir(TRANSFERENCIA novo)                         
    {                        
        if (estado.transfereReserva == novo) return;                        
        estado.transfereReserva = novo;                        
        debugVariavel("TRANSFERENCIA transfereReserva : ", texto(novo), NENHUM, VERDE, AMARELO);                        
    }                        
                        
    TRANSFERENCIA atual()                        
    {                        
         return estado.transfereReserva;                         
    }                        
                        
    const __FlashStringHelper* texto(TRANSFERENCIA a)                        
    {                        
        switch(a)                        
        {                        
            case TRANSFERENCIA::NORMAL:                               return F("NORMAL");                        
            case TRANSFERENCIA::LIBERADO:                             return F("LIBERADO");        
            case TRANSFERENCIA::SO_MAIS_UM_POUQUINHO:                 return F("SO_MAIS_UM_POUQUINHO");
            case TRANSFERENCIA::FINALIZADO:                           return F("FINALIZADO");                             
            case TRANSFERENCIA::BLOQUEIO_SEGURANCA:                   return F("BLOQUEIO_SEGURANCA");                        
                        
            default:                return F("DESCONHECIDO");                        
        }                        
    }                        
}                        
                         
// [FIM::BAT::RESERVATORIO]                        
                           
// [INICIO::BAT::BOMBA_12V]      
      
//====================================================================================      
//                   BOMBA_12V      
//====================================================================================      
      
namespace BOMBA_12V      
{      
    void definir(ESTAGIOS novo)       
    {      
        if (estado.bombaReserva == novo) return;      
        estado.bombaReserva = novo;      
        debugVariavel("ESTAGIOS bombaReserva : ", texto(novo), NENHUM, VERDE, AMARELO);      
    }      
      
    ESTAGIOS atual()      
    {      
         return estado.bombaReserva;       
    }      
      
    const __FlashStringHelper* texto(ESTAGIOS a)      
    {      
        switch(a)      
        {      
            case ESTAGIOS::REPOUSO:                                   return F("REPOUSO");     
            case ESTAGIOS::LIBERADO:                                  return F("LIBERADO");     
            case ESTAGIOS::AGUARDA_LIBERACAO:                        return F("AGUARDA_LIBERACAO");        
            case ESTAGIOS::LIGANDO_BOMBA:                             return F("LIGANDO_BOMBA");     
            case ESTAGIOS::BOMBA_LIGADA:                              return F("BOMBA_LIGADA");     
            case ESTAGIOS::DESLIGANDO_BOMBA:                          return F("DESLIGANDO_BOMBA");        
      
            default:                return F("DESCONHECIDO");      
        }      
    }      
}      
       
// [FIM::BAT::BOMBA_12V]      
                 
// [INICIO::BAT::SISTEMA]              
              
//====================================================================================              
//                   SISTEMA              
//====================================================================================              
              
namespace SISTEMA_ECO_HIDRO              
{              
    void definir(MODO novo)               
    {              
        if (estado.sistemaEcoHidroModo == novo) return;              
        estado.sistemaEcoHidroModo = novo;              
        debugVariavel("MODO sistemaEcoHidroModo : ", texto(novo), NENHUM, VERDE, AMARELO);              
    }              
              
    MODO atual()              
    {              
         return estado.sistemaEcoHidroModo;               
    }              
              
    const __FlashStringHelper* texto(MODO a)              
    {              
        switch(a)              
        {              
            case MODO::NORMAL:                                        return F("NORMAL");             
            case MODO::CICLO_COMPLETO:                                return F("CICLO_COMPLETO");             
            case MODO::MEIO_CICLO:                                    return F("MEIO_CICLO");             
            case MODO::BLOQUEIO:                                      return F("BLOQUEIO");             
            case MODO::CONTADOR_PULSO:                                return F("CONTADOR_PULSO");             
              
            default:                return F("DESCONHECIDO");              
        }              
    }              
}              
               
// [FIM::BAT::SISTEMA]              
                                
// [INICIO::BAT::LUZ_ESTACIONAMENTO]                     
                     
//====================================================================================                     
//                   LUZ_ESTACIONAMENTO                     
//====================================================================================                     
                     
namespace LUZ_ESTACIONAMENTO                     
{                     
    void definir(SITUACAO novo)                      
    {                     
        if (estado.luzEstacionamento == novo) return;                     
        estado.luzEstacionamento = novo;                     
        debugVariavel("SITUACAO luzEstacionamento : ", texto(novo), NENHUM, VERDE, AMARELO);                     
    }                     
                     
    SITUACAO atual()                     
    {                     
         return estado.luzEstacionamento;                      
    }                     
                     
    const __FlashStringHelper* texto(SITUACAO a)                     
    {                     
        switch(a)                     
        {                     
            case SITUACAO::DESLIGADO:                                 return F("DESLIGADO");  
            case SITUACAO::DESLIGANDO:                                return F("DESLIGANDO");                  
            case SITUACAO::TEMPORIZADO:                               return F("TEMPORIZADO");                     
            case SITUACAO::FORCADO:                                   return F("FORCADO");
            case SITUACAO::CONSUMINDO:                                return F("CONSUMINDO");                     
                     
            default:                return F("DESCONHECIDO");                     
        }                     
    }                     
}                     
                      
// [FIM::BAT::LUZ_ESTACIONAMENTO]                     
                                   
// [INICIO::BAT::LUZ_ACESSO]                 
                 
//====================================================================================                 
//                   LUZ_ACESSO                 
//====================================================================================                 
                 
namespace LUZ_ACESSO                 
{                 
    void definir(SITUACAO novo)                  
    {                 
        if (estado.luzReservatorio == novo) return;                 
        estado.luzReservatorio = novo;                 
        debugVariavel("SITUACAO luzReservatorio : ", texto(novo), NENHUM, VERDE, AMARELO);                 
    }                 
                 
    SITUACAO atual()                 
    {                 
         return estado.luzReservatorio;                  
    }                 
                 
    const __FlashStringHelper* texto(SITUACAO a)                 
    {                 
        switch(a)                 
        {                 
            case SITUACAO::DESLIGADO:                                 return F("DESLIGADO");  
            case SITUACAO::DESLIGANDO:                                return F("DESLIGANDO");              
            case SITUACAO::TEMPORIZADO:                               return F("TEMPORIZADO");
            case SITUACAO::FORCADO:                                   return F("FORCADO");
            case SITUACAO::CONSUMINDO:                                return F("CONSUMINDO");           
                 
            default:                return F("DESCONHECIDO");                 
        }                 
    }                 
}                 
                  
// [FIM::BAT::LUZ_ACESSO]                 
                                 
// [INICIO::BAT::VOLTAGEM]                   
                   
//====================================================================================                   
//                   VOLTAGEM                   
//====================================================================================                   
                   
namespace VOLTAGEM                   
{                   
    void definir(LEITURA novo)                    
    {                   
        if (estado.bateria == novo) return;                   
        estado.bateria = novo;                   
        debugVariavel("LEITURA bateria : ", texto(novo), NENHUM, VERDE, AMARELO);                   
    }                   
                   
    LEITURA atual()                   
    {                   
         return estado.bateria;                    
    }                   
                   
    const __FlashStringHelper* texto(LEITURA a)                   
    {                   
        switch(a)                   
        {                   
            case LEITURA::NORMAL:                                     return F("NORMAL");                   
            case LEITURA::IDEAL:                                      return F("IDEAL");                   
            case LEITURA::CHEIA:                                      return F("CHEIA");
            case LEITURA::MANTENDO_CARGA:                             return F("MANTENDO_CARGA"); 
            case LEITURA::CARGA_CONSUMIDA:                            return F("CARGA_CONSUMIDA");                  
            case LEITURA::CRITICO:                                    return F("CRITICO");                   
                   
            default:                return F("DESCONHECIDO");                   
        }                   
    }                   
}                   
                    
// [FIM::BAT::VOLTAGEM]                   
                       
// [INICIO::BAT::DESLIGAMENTO_BOMBA]       
       
//====================================================================================       
//                   DESLIGAMENTO_BOMBA       
//====================================================================================       
       
namespace DESLIGAMENTO_BOMBA       
{       
    void definir(MOTIVO novo)        
    {       
        if (estado.motivoBomba == novo) return;       
        estado.motivoBomba = novo;       
        debugVariavel("MOTIVO motivoBomba : ", texto(novo), NENHUM, VERDE, AMARELO);       
    }       
       
    MOTIVO atual()       
    {       
         return estado.motivoBomba;        
    }       
       
    const __FlashStringHelper* texto(MOTIVO a)       
    {       
        switch(a)       
        {       
            case MOTIVO::NENHUM:                                      return F("NENHUM");      
            case MOTIVO::INICIADO_BOMBA_CASA:                         return F("INICIADO_BOMBA_CASA");      
            case MOTIVO::BOMBA_OK:                                    return F("BOMBA_OK");      
            case MOTIVO::CASA_MINIMA:                                 return F("CASA_MINIMA");      
            case MOTIVO::TORRE_MAXIMA:                                return F("TORRE_MAXIMA");      
            case MOTIVO::ESTOQUE_MINIMO:                              return F("ESTOQUE_MINIMO");      
            case MOTIVO::DHT11_TEMPERATURA:                           return F("DHT11_TEMPERATURA");
            case MOTIVO::DHT11_ERRO:                                  return F("DHT11_ERRO");     
            case MOTIVO::SENSOR_IR_BOMBA_TRAVADO:                     return F("SENSOR_IR_BOMBA_TRAVADO");      
            case MOTIVO::TEMPO_MAXIMO:                                return F("TEMPO_MAXIMO");      
            case MOTIVO::RESIDENCIAL_ORDENOU:                         return F("RESIDENCIAL_ORDENOU");      
       
            default:                return F("DESCONHECIDO");       
        }       
    }       
}       
        
// [FIM::BAT::DESLIGAMENTO_BOMBA]       
                   
// [INICIO::BAT::PORTAO_ELETRONICO]                 
                 
//====================================================================================                 
//                   PORTAO_ELETRONICO                 
//====================================================================================                 
                 
namespace PORTAO_ELETRONICO                 
{                 
    void definir(SITUACAO novo)                  
    {                 
        if (estado.portao == novo) return;                 
        estado.portao = novo;                 
        debugVariavel("SITUACAO portao : ", texto(novo), NENHUM, VERDE, AMARELO);                 
    }                 
                 
    SITUACAO atual()                 
    {                 
         return estado.portao;                  
    }                 
                 
    const __FlashStringHelper* texto(SITUACAO a)                 
    {                 
        switch(a)                 
        {                 
            case SITUACAO::REPOUSO:                                   return F("REPOUSO");                 
            case SITUACAO::LIBERADO:                                  return F("LIBERADO");                 
            case SITUACAO::MANTEM_ABERTO:                             return F("MANTEM_ABERTO");                 
            case SITUACAO::FINALIZADO:                                return F("FINALIZADO");                 
                 
            default:                return F("DESCONHECIDO");                 
        }                 
    }                 
}                 
                  
// [FIM::BAT::PORTAO_ELETRONICO]                          

// [INICIO::BAT::UNIDADE_POTENCIA]

//====================================================================================
//                   UNIDADE_POTENCIA (Controle Central do Inversor)
//====================================================================================

namespace UNIDADE_POTENCIA
{
    void definir(ESTAGIOS novo)
    {
        if (estado.unidadePotencia == novo) return;
        estado.unidadePotencia = novo;
        debugVariavel("ESTAGIOS unidadePotencia : ", texto(novo), NENHUM, VERDE, AMARELO);
    }

    ESTAGIOS atual()
    {
         return estado.unidadePotencia;
    }

    const __FlashStringHelper* texto(ESTAGIOS a)
    {
        switch(a)
        {
            case ESTAGIOS::REPOUSO:                                     return F("REPOUSO");
            case ESTAGIOS::ACIONAMENTO_RELES:                           return F("ACIONAMENTO_RELES");
            case ESTAGIOS::AGUARDANDO_ESTABILIZAR:                      return F("AGUARDANDO_ESTABILIZAR");
            case ESTAGIOS::LIGADO:                                      return F("LIGADO");
            case ESTAGIOS::DESLIGANDO:                                  return F("DESLIGANDO");

            default:                return F("DESCONHECIDO");
        }
    }
}

// [FIM::BAT::UNIDADE_POTENCIA]

// [INICIO::BAT::POTENCIA]

//====================================================================================
//                   POTENCIA (Selecionador de Potência dos aparelhos)
//====================================================================================

namespace POTENCIA
{
    void definir(SELECIONADO novo)
    {
        if (estado.potenciaSelecionado == novo) return;
        estado.potenciaSelecionado = novo;
        debugVariavel("SELECIONADO potencia : ", texto(novo), NENHUM, VERDE, AMARELO);
    }

    SELECIONADO atual()
    {
         return estado.potenciaSelecionado;
    }

    const __FlashStringHelper* texto(SELECIONADO a)
    {
        switch(a)
        {
            case SELECIONADO::REPOUSO:                                   return F("REPOUSO");
            case SELECIONADO::BOMBA_CASA:                                return F("BOMBA_CASA");
            case SELECIONADO::BOMBA_12V:                                 return F("BOMBA_12V");
            case SELECIONADO::PORTAO:                                    return F("PORTAO");
            case SELECIONADO::DESLIGANDO:                                return F("DESLIGANDO");

            default:                return F("DESCONHECIDO");
        }
    }
}

// [FIM::BAT::POTENCIA]

// [INICIO::BAT::ORDENS_DRIMEL_SYSTEM::CASA_TORRE]

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
            debugVariavel("ORDEM ordemCasaTorre : ", texto(novo), NENHUM, VERDE, AMARELO);
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
}

// [FIM::BAT::ORDENS_DRIMEL_SYSTEM::CASA_TORRE]

// [INSERE::BAT::ESTADOS_CPP_Eco_Hidro]