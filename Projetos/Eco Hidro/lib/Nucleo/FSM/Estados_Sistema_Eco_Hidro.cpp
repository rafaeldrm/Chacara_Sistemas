//====================================================================================                              
//                   DEPENDÊNCIAS                              
//====================================================================================                              
                                                       
#include "FSM/Estados_Sistema_Eco_Hidro.h"                              
#include "DEBUG_MODE.h"  

#include "Rastreador_Trocas.h"
                              
//====================================================================================                              
//                   VARIÁVEIS INTERNAS (ESTÁTICAS)                              
//====================================================================================                              
                              
struct estadosSistemaEcoHidro                               
{                              
                              
        CASA_TORRE::ETAPAS casaTorre; // [VAR::CASA_TORRE]   
        BOMBA_CASA::ESTAGIOS bombaCasa; // [VAR::BOMBA_CASA]     
        RESERVATORIO::TRANSFERENCIA reservatorio; // [VAR::RESERVATORIO]                           
        BOMBA_12V::ESTAGIOS bomba12v; // [VAR::BOMBA_12V]        
        SISTEMA_ECO_HIDRO::MODO sistemaModo; // [VAR::SISTEMA_ECO_HIDRO]                
        LUZ_ESTACIONAMENTO::SITUACAO luzEstacionamento; // [VAR::LUZ_ESTACIONAMENTO]                        
        LUZ_ACESSO::SITUACAO luzReservatorio; // [VAR::LUZ_ACESSO]                   
        VOLTAGEM::LEITURA bateria; // [VAR::VOLTAGEM]                      
        DESLIGAMENTO_BOMBA::MOTIVO motivoBomba; // [VAR::DESLIGAMENTO_BOMBA]         
        PORTAO::ESTAGIOS portao; // [VAR::PORTAO_ELETRONICO]                                    
        UNIDADE_POTENCIA::ESTAGIOS unidadePotencia; // [VAR::UNIDADE_POTENCIA]   
        POTENCIA::SELECIONADO potenciaSelecionado; // [VAR::POTENCIA]                      
        TRANCO_BOMBA_CASA::ESTAGIOS trancoBomba; // [VAR::TRANCO_BOMBA_CASA]
        PROTECAO_TERMICA_BOMBA_CASA::SITUACAO termicaBomba; //[VAR::PROTECAO_TERMICA_BOMBA_CASA]
        GIRO_PREVENTIVO_BOMBA_CASA::BOMBEAMENTO conservacaoBomba; //[VAR::GIRO_PROTECAO_BOMBA_CASA]
    // [STRUCT::BAT]
                              
};                              
                              
static estadosSistemaEcoHidro estado =                               
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
    PORTAO::ESTAGIOS::REPOUSO, // [INIT::PORTAO_ELETRONICO]                
    UNIDADE_POTENCIA::ESTAGIOS::REPOUSO,// [INIT::UNIDADE_POTENCIA]   
    POTENCIA::SELECIONADO::REPOUSO, // [INIT::POTENCIA]                         
    TRANCO_BOMBA_CASA::ESTAGIOS::REPOUSO, // [INIT::TRANCO_BOMBA_CASA]
    PROTECAO_TERMICA_BOMBA_CASA::SITUACAO::REPOUSO, //[INIT::PROTECAO_TERMICA_BOMBA_CASA]
    GIRO_PREVENTIVO_BOMBA_CASA::BOMBEAMENTO::INICIA_CONTAGEM_COTIDIANO//[VAR::VIRGULA] //[INIT::GIRO_PROTECAO_BOMBA_CASA]
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
    // Estado atual real e o próximo estado desejado para o próximo tick
    static ETAPAS estadoAtual = ETAPAS::NORMAL;
    static ETAPAS proximoEstado = ETAPAS::NORMAL;
    static bool solicitacaoPendente = false; 

    void definir(ETAPAS novo)
    {
        if (estadoAtual == novo) return;
        
        // Em vez de mudar na hora, apenas agenda para o final/início do ciclo
        proximoEstado = novo;
        solicitacaoPendente = true;
    }

    // Função chamada controladamente no final ou início do loop principal
    void atualizarCiclo() {
        if (!solicitacaoPendente) return;
        
        if (estadoAtual != proximoEstado) {
            ETAPAS antigo = estadoAtual;
            estadoAtual = proximoEstado;
            
            // O log e o acionamento físico acontecem de forma síncrona e controlada aqui
            debugVariavel("CASA TORRE : ", texto(estadoAtual), LINHA, VERDE, AMARELO);
        }
        solicitacaoPendente = false;
    }
                 
    ETAPAS atual()                    
    {                    
         return estadoAtual;                     
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
            case ETAPAS::LIBERADO_PARA_O_RESERVATORIO:                return F("LIBERADO_PARA_O_RESERVATORIO");
            case ETAPAS::BLOQUEIO:                                    return F("BLOQUEIO");   
    
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
   // Estado atual real e o próximo estado desejado para o próximo tick
    static ESTAGIOS estadoAtual = ESTAGIOS::REPOUSO;
    static ESTAGIOS proximoEstado = ESTAGIOS::REPOUSO;
    static bool solicitacaoPendente = false;

    void definir(ESTAGIOS novo)
    {
        if (estadoAtual == novo) return;
        
        // Em vez de mudar na hora, apenas agenda para o final/início do ciclo
        proximoEstado = novo;
        solicitacaoPendente = true;
    }

    // Função chamada controladamente no final ou início do loop principal
    void atualizarCiclo() {
        if (!solicitacaoPendente) return;
        
        if (estadoAtual != proximoEstado) {
            ESTAGIOS antigo = estadoAtual;
            estadoAtual = proximoEstado;
            
            // O log e o acionamento físico acontecem de forma síncrona e controlada aqui
            debugVariavel("BOMBA CASA : ", texto(estadoAtual), LINHA, VERDE, AMARELO);
        }
        solicitacaoPendente = false;
    }
                 
    ESTAGIOS atual()                    
    {                    
         return estadoAtual;                     
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
            case ESTAGIOS::BLOQUEIO:                                  return F("BLOQUEIO");
            case ESTAGIOS::BLOQUEADO:                                 return F("BLOQUEADO");      
      
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
    // Estado atual real e o próximo estado desejado para o próximo tick
    static TRANSFERENCIA estadoAtual = TRANSFERENCIA::NORMAL;
    static TRANSFERENCIA proximoEstado = TRANSFERENCIA::NORMAL;
    static bool solicitacaoPendente = false; 

    void definir(TRANSFERENCIA novo)
    {
        if (estadoAtual == novo) return;
        
        // Em vez de mudar na hora, apenas agenda para o final/início do ciclo
        proximoEstado = novo;
        solicitacaoPendente = true;
    }

    // Função chamada controladamente no final ou início do loop principal
    void atualizarCiclo() {
        if (!solicitacaoPendente) return;
        
        if (estadoAtual != proximoEstado) {
            TRANSFERENCIA antigo = estadoAtual;
            estadoAtual = proximoEstado;
            
            // O log e o acionamento físico acontecem de forma síncrona e controlada aqui
            debugVariavel("RESERVATORIO : ", texto(estadoAtual), LINHA, VERDE, AMARELO);
        }
        solicitacaoPendente = false;
    }
                 
    TRANSFERENCIA atual()                    
    {                    
         return estadoAtual;                     
    }                         
                           
    const __FlashStringHelper* texto(TRANSFERENCIA a)                           
    {                           
        switch(a)                           
        {                           
            case TRANSFERENCIA::NORMAL:                               return F("NORMAL");                           
            case TRANSFERENCIA::LIBERADO:                             return F("LIBERADO");
            case TRANSFERENCIA::TRANSFERENCIA_RESERVATORIOS:          return F("TRANSFERENCIA_RESERVATORIOS");         
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
    // Estado atual real e o próximo estado desejado para o próximo tick
    static ESTAGIOS estadoAtual = ESTAGIOS::REPOUSO;
    static ESTAGIOS proximoEstado = ESTAGIOS::REPOUSO;
    static bool solicitacaoPendente = false; 

    void definir(ESTAGIOS novo)
    {
        if (estadoAtual == novo) return;
        
        // Em vez de mudar na hora, apenas agenda para o final/início do ciclo
        proximoEstado = novo;
        solicitacaoPendente = true;
    }

    // Função chamada controladamente no final ou início do loop principal
    void atualizarCiclo() {
        if (!solicitacaoPendente) return;
        
        if (estadoAtual != proximoEstado) {
            ESTAGIOS antigo = estadoAtual;
            estadoAtual = proximoEstado;
            
            // O log e o acionamento físico acontecem de forma síncrona e controlada aqui
            debugVariavel("BOMBA 12V : ", texto(estadoAtual), LINHA, VERDE, AMARELO);
        }
        solicitacaoPendente = false;
    }
                 
    ESTAGIOS atual()                    
    {                    
         return estadoAtual;                     
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
    // Estado atual real e o próximo estado desejado para o próximo tick
    static MODO estadoAtual = MODO::NORMAL;
    static MODO proximoEstado = MODO::NORMAL;
    static bool solicitacaoPendente = false; 

    void definir(MODO novo)
    {
        if (estadoAtual == novo) return;
        
        // Em vez de mudar na hora, apenas agenda para o final/início do ciclo
        proximoEstado = novo;
        solicitacaoPendente = true;
    }

    // Função chamada controladamente no final ou início do loop principal
    void atualizarCiclo() {
        if (!solicitacaoPendente) return;
        
        if (estadoAtual != proximoEstado) {
            MODO antigo = estadoAtual;
            estadoAtual = proximoEstado;
            
            // O log e o acionamento físico acontecem de forma síncrona e controlada aqui
            debugVariavel("SISTEMA ECO HIDRO : ", texto(estadoAtual), LINHA, VERDE, AMARELO);
        }
        solicitacaoPendente = false;
    }
                 
    MODO atual()                    
    {                    
         return estadoAtual;                     
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
   // Estado atual real e o próximo estado desejado para o próximo tick
    static SITUACAO estadoAtual = SITUACAO::DESLIGADO;
    static SITUACAO proximoEstado = SITUACAO::DESLIGADO;
    static bool solicitacaoPendente = false; 

    void definir(SITUACAO novo)
    {
        if (estadoAtual == novo) return;
        
        // Em vez de mudar na hora, apenas agenda para o final/início do ciclo
        proximoEstado = novo;
        solicitacaoPendente = true;
    }

    // Função chamada controladamente no final ou início do loop principal
    void atualizarCiclo() {
        if (!solicitacaoPendente) return;
        
        if (estadoAtual != proximoEstado) {
            SITUACAO antigo = estadoAtual;
            estadoAtual = proximoEstado;
            
            // O log e o acionamento físico acontecem de forma síncrona e controlada aqui
            debugVariavel("LUZ ESTACIONAMENTO : ", texto(estadoAtual), LINHA, VERDE, AMARELO);
        }
        solicitacaoPendente = false;
    }
                 
    SITUACAO atual()                    
    {                    
         return estadoAtual;                     
    }                
                        
    const __FlashStringHelper* texto(SITUACAO a)                        
    {                        
        switch(a)                        
        {                        
            case SITUACAO::DESLIGADO:                                 return F("DESLIGADO");     
            case SITUACAO::DESLIGANDO:                                return F("DESLIGANDO");
            case SITUACAO::INICIANDO_TEMPORIZACAO:                    return F("INICIANDO_TEMPORIZACAO");                     
            case SITUACAO::TEMPORIZADO:                               return F("TEMPORIZADO");                        
            case SITUACAO::FORCADO:                                   return F("FORCADO");   
            case SITUACAO::CONSUMINDO:                                return F("CONSUMINDO");
            case SITUACAO::ACIONADO:                                  return F("ACIONADO");                        
                        
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
   // Estado atual real e o próximo estado desejado para o próximo tick
    static SITUACAO estadoAtual = SITUACAO::DESLIGADO;
    static SITUACAO proximoEstado = SITUACAO::DESLIGADO;
    static bool solicitacaoPendente = false;

    void definir(SITUACAO novo)
    {
        if (estadoAtual == novo) return;
        
        // Em vez de mudar na hora, apenas agenda para o final/início do ciclo
        proximoEstado = novo;
        solicitacaoPendente = true;
    }

    // Função chamada controladamente no final ou início do loop principal
    void atualizarCiclo() {
        if (!solicitacaoPendente) return;
        
        if (estadoAtual != proximoEstado) {
            SITUACAO antigo = estadoAtual;
            estadoAtual = proximoEstado;
            
            // O log e o acionamento físico acontecem de forma síncrona e controlada aqui
            debugVariavel("LUZ ACESSO RESERVATÓRIO : ", texto(estadoAtual), LINHA, VERDE, AMARELO);
        }
        solicitacaoPendente = false;
    }
                 
    SITUACAO atual()                    
    {                    
         return estadoAtual;                     
    }            
                    
    const __FlashStringHelper* texto(SITUACAO a)                    
    {                    
        switch(a)                    
        {                    
            case SITUACAO::DESLIGADO:                                 return F("DESLIGADO");     
            case SITUACAO::DESLIGANDO:                                return F("DESLIGANDO");
            case SITUACAO::INICIANDO_TEMPORIZACAO:                    return F("INICIANDO_TEMPORIZACAO");                 
            case SITUACAO::TEMPORIZADO:                               return F("TEMPORIZADO");   
            case SITUACAO::FORCADO:                                   return F("FORCADO");   
            case SITUACAO::CONSUMINDO:                                return F("CONSUMINDO");
            case SITUACAO::ACIONADO:                                  return F("ACIONADO");              
                    
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
 // Estado atual real e o próximo estado desejado para o próximo tick
    static LEITURA estadoAtual = LEITURA::NORMAL;
    static LEITURA proximoEstado = LEITURA::NORMAL;
    static bool solicitacaoPendente = false;

    void definir(LEITURA novo)
    {
        if (estadoAtual == novo) return;
        
        // Em vez de mudar na hora, apenas agenda para o final/início do ciclo
        proximoEstado = novo;
        solicitacaoPendente = true;
    }

    // Função chamada controladamente no final ou início do loop principal
    void atualizarCiclo() {
        if (!solicitacaoPendente) return;
        
        if (estadoAtual != proximoEstado) {
            LEITURA antigo = estadoAtual;
            estadoAtual = proximoEstado;
            
            // O log e o acionamento físico acontecem de forma síncrona e controlada aqui
            debugVariavel("VOLTAGEM : ", texto(estadoAtual), LINHA, VERDE, AMARELO);
        }
        solicitacaoPendente = false;
    }
                 
    LEITURA atual()                    
    {                    
         return estadoAtual;                     
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
            case LEITURA::CARGA_PRIORIDADE:              return F("CARGA_PRIORIDADE");                     
                      
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
    // Estado atual real e o próximo estado desejado para o próximo tick
    static MOTIVO estadoAtual = MOTIVO::NENHUM;
    static MOTIVO proximoEstado = MOTIVO::NENHUM;
    static bool solicitacaoPendente = false; 

    void definir(MOTIVO novo)
    {
        if (estadoAtual == novo) return;
        
        // Em vez de mudar na hora, apenas agenda para o final/início do ciclo
        proximoEstado = novo;
        solicitacaoPendente = true;
    }

    // Função chamada controladamente no final ou início do loop principal
    void atualizarCiclo() {
        if (!solicitacaoPendente) return;
        
        if (estadoAtual != proximoEstado) {
            MOTIVO antigo = estadoAtual;
            estadoAtual = proximoEstado;
            
            // O log e o acionamento físico acontecem de forma síncrona e controlada aqui
            debugVariavel("CASA TORRE : ", texto(estadoAtual), LINHA, VERDE, AMARELO);
        }
        solicitacaoPendente = false;
    }
                 
    MOTIVO atual()                    
    {                    
         return estadoAtual;                     
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
                    
namespace PORTAO                    
{                    
    // Estado atual real e o próximo estado desejado para o próximo tick
    static ESTAGIOS estadoAtual = ESTAGIOS::REPOUSO;
    static ESTAGIOS proximoEstado = ESTAGIOS::REPOUSO;
    static bool solicitacaoPendente = false; 

    void definir(ESTAGIOS novo)
    {
        if (estadoAtual == novo) return;
        
        // Em vez de mudar na hora, apenas agenda para o final/início do ciclo
        proximoEstado = novo;
        solicitacaoPendente = true;
    }

    // Função chamada controladamente no final ou início do loop principal
    void atualizarCiclo() {
        if (!solicitacaoPendente) return;
        
        if (estadoAtual != proximoEstado) {
            ESTAGIOS antigo = estadoAtual;
            estadoAtual = proximoEstado;
            
            // O log e o acionamento físico acontecem de forma síncrona e controlada aqui
            debugVariavel("PORTAO : ", texto(estadoAtual), LINHA, VERDE, AMARELO);
        }
        solicitacaoPendente = false;
    }
                 
    ESTAGIOS atual()                    
    {                    
         return estadoAtual;                     
    }            
    const __FlashStringHelper* texto(ESTAGIOS a)                    
    {                    
        switch(a)                    
        {                    
            case ESTAGIOS::REPOUSO:                                   return F("REPOUSO");                    
            case ESTAGIOS::LIBERADO:                                  return F("LIBERADO");                    
            case ESTAGIOS::AGUARDA_LIBERACAO:                         return F("AGUARDA_LIBERACAO");                    
            case ESTAGIOS::LIGANDO_PORTAO:                            return F("LIGANDO_PORTAO");
            case ESTAGIOS::PORTAO_LIGADO:                             return F("PORTAO_LIGADO");
            case ESTAGIOS::DESLIGANDO_PORTAO:                         return F("DESLIGANDO_PORTAO");                    
                    
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
   // Estado atual real e o próximo estado desejado para o próximo tick
    static ESTAGIOS estadoAtual = ESTAGIOS::REPOUSO;
    static ESTAGIOS proximoEstado = ESTAGIOS::REPOUSO;
    static bool solicitacaoPendente = false;

    void definir(ESTAGIOS novo)
    {
        if (estadoAtual == novo) return;
        
        // Em vez de mudar na hora, apenas agenda para o final/início do ciclo
        proximoEstado = novo;
        solicitacaoPendente = true;
    }

    // Função chamada controladamente no final ou início do loop principal
    void atualizarCiclo() {
        if (!solicitacaoPendente) return;
        
        if (estadoAtual != proximoEstado) {
            ESTAGIOS antigo = estadoAtual;
            estadoAtual = proximoEstado;
            
            // O log e o acionamento físico acontecem de forma síncrona e controlada aqui
            debugVariavel("UNIDADE POTENCIA : ", texto(estadoAtual), LINHA, VERDE, AMARELO);
        }
        solicitacaoPendente = false;
    }
                 
    ESTAGIOS atual()                    
    {                    
         return estadoAtual;                     
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
  // Estado atual real e o próximo estado desejado para o próximo tick
    static SELECIONADO estadoAtual = SELECIONADO::REPOUSO;
    static SELECIONADO proximoEstado = SELECIONADO::REPOUSO;
    static bool solicitacaoPendente = false;

    void definir(SELECIONADO novo)
    {
        if (estadoAtual == novo) return;
        
        // Em vez de mudar na hora, apenas agenda para o final/início do ciclo
        proximoEstado = novo;
        solicitacaoPendente = true;
    }

    // Função chamada controladamente no final ou início do loop principal
    void atualizarCiclo() {
        if (!solicitacaoPendente) return;
        
        if (estadoAtual != proximoEstado) {
            SELECIONADO antigo = estadoAtual;
            estadoAtual = proximoEstado;
            
            // O log e o acionamento físico acontecem de forma síncrona e controlada aqui
            debugVariavel("POTENCIA : ", texto(estadoAtual), LINHA, VERDE, AMARELO);
        }
        solicitacaoPendente = false;
    }
                 
    SELECIONADO atual()                    
    {                    
         return estadoAtual;                     
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
   
// [INICIO::BAT::TRANCO_BOMBA_CASA]  
  
//====================================================================================  
//                   TRANCO_BOMBA_CASA  
//====================================================================================  
  
namespace TRANCO_BOMBA_CASA  
{  
    // Estado atual real e o próximo estado desejado para o próximo tick
    static ESTAGIOS estadoAtual = ESTAGIOS::REPOUSO;
    static ESTAGIOS proximoEstado = ESTAGIOS::REPOUSO;
    static bool solicitacaoPendente = false; 

    void definir(ESTAGIOS novo)
    {
        if (estadoAtual == novo) return;
        
        // Em vez de mudar na hora, apenas agenda para o final/início do ciclo
        proximoEstado = novo;
        solicitacaoPendente = true;
    }

    // Função chamada controladamente no final ou início do loop principal
    void atualizarCiclo() {
        if (!solicitacaoPendente) return;
        
        if (estadoAtual != proximoEstado) {
            ESTAGIOS antigo = estadoAtual;
            estadoAtual = proximoEstado;
            
            // O log e o acionamento físico acontecem de forma síncrona e controlada aqui
            debugVariavel("TRANCO BOMBA : ", texto(estadoAtual), LINHA, VERDE, AMARELO);
        }
        solicitacaoPendente = false;
    }
                  
    ESTAGIOS atual()                    
    {                    
         return estadoAtual;                     
    }
  
    const __FlashStringHelper* texto(ESTAGIOS a)  
    {  
        switch(a)  
        {  
            case ESTAGIOS::REPOUSO:                                   return F("REPOUSO");  
            case ESTAGIOS::INICIANDO_MONITORAMENTO_HELICE:            return F("INICIANDO_MONITORAMENTO_HELICE");  
            case ESTAGIOS::BOMBA_TRAVADA:                             return F("BOMBA_TRAVADA");  
            case ESTAGIOS::PRIMEIRA_TENTATIVA_TRANCO:                 return F("PRIMEIRA_TENTATIVA_TRANCO");  
            case ESTAGIOS::SEGUNDA_TENTATIVA_TRANCO:                  return F("SEGUNDA_TENTATIVA_TRANCO");   
            case ESTAGIOS::SENSOR_IR_DESATIVADO:                      return F("SENSOR_IR_DESATIVADO");  
            case ESTAGIOS::BOMBA_LIBERADA:                            return F("BOMBA_LIBERADA");
  
            default:                return F("DESCONHECIDO");  
        }  
    }  
} 
   
// [FIM::BAT::TRANCO_BOMBA_CASA] 

// [INICIO::BAT::PROTECAO_TERMICA_BOMBA_CASA]

//====================================================================================  
//                   PROTECAO_TERMICA_BOMBA_CASA  
//====================================================================================  
  
namespace PROTECAO_TERMICA_BOMBA_CASA  
{  
    // Estado atual real e o próximo estado desejado para o próximo tick
    static SITUACAO estadoAtual = SITUACAO::REPOUSO;
    static SITUACAO proximoEstado = SITUACAO::REPOUSO;
    static bool solicitacaoPendente = false; 

    void definir(SITUACAO novo)
    {
        if (estadoAtual == novo) return;
        
        // Em vez de mudar na hora, apenas agenda para o final/início do ciclo
        proximoEstado = novo;
        solicitacaoPendente = true;
    }

    // Função chamada controladamente no final ou início do loop principal
    void atualizarCiclo() {
        if (!solicitacaoPendente) return;
        
        if (estadoAtual != proximoEstado) {
            SITUACAO antigo = estadoAtual;
            estadoAtual = proximoEstado;
            
            // O log e o acionamento físico acontecem de forma síncrona e controlada aqui
            debugVariavel("PROTECAO TERMICA BOMBA : ", texto(estadoAtual), LINHA, VERDE, AMARELO);
        }
        solicitacaoPendente = false;
    }
                  
    SITUACAO atual()                    
    {                    
         return estadoAtual;                     
    }
  
    const __FlashStringHelper* texto(SITUACAO a)  
    {  
        switch(a)  
        {  
            case SITUACAO::REPOUSO:                                   return F("REPOUSO");  
            case SITUACAO::MONITORAMENTO_ATIVADO:                     return F("MONITORAMENTO_ATIVADO");  
            case SITUACAO::SUPERAQUECIMENTO:                          return F("SUPERAQUECIMENTO");  
            case SITUACAO::MODO_SEGURANCA_PRIMEIRO_CICLO:             return F("MODO_SEGURANCA_PRIMEIRO_CICLO");  
            case SITUACAO::MODO_SEGURANCA_SEGUNDO_CICLO:              return F("MODO_SEGURANCA_SEGUNDO_CICLO");   
            case SITUACAO::FINALIZADO:                                return F("FINALIZADO");  
            
            default:                return F("DESCONHECIDO");  
        }  
    }  
} 

// [FIM::BAT::PROTECAO_TERMICA_BOMBA_CASA]
   

// [INICIO::BAT::GIRO_PROTECAO_BOMBA_CASA]

//====================================================================================  
//                   GIRO_PROTECAO_BOMBA_CASA 
//====================================================================================  
  
namespace GIRO_PREVENTIVO_BOMBA_CASA 
{  
    // Estado atual real e o próximo estado desejado para o próximo tick
    static BOMBEAMENTO estadoAtual = BOMBEAMENTO::INICIA_CONTAGEM_COTIDIANO;
    static BOMBEAMENTO proximoEstado = BOMBEAMENTO::INICIA_CONTAGEM_COTIDIANO;
    static bool solicitacaoPendente = false; 

    void definir(BOMBEAMENTO novo)
    {
        if (estadoAtual == novo) return;
        
        // Em vez de mudar na hora, apenas agenda para o final/início do ciclo
        proximoEstado = novo;
        solicitacaoPendente = true;
    }

    // Função chamada controladamente no final ou início do loop principal
    void atualizarCiclo() {
        if (!solicitacaoPendente) return;
        
        if (estadoAtual != proximoEstado) {
            BOMBEAMENTO antigo = estadoAtual;
            estadoAtual = proximoEstado;
            
            // O log e o acionamento físico acontecem de forma síncrona e controlada aqui
            debugVariavel("GIRO PREVENTIVO BOMBA CASA : ", texto(estadoAtual), LINHA, VERDE, AMARELO);
        }
        solicitacaoPendente = false;
    }
                  
    BOMBEAMENTO atual()                    
    {                    
         return estadoAtual;                     
    }
  
    const __FlashStringHelper* texto(BOMBEAMENTO a)  
    {  
        switch(a)  
        {  
            case BOMBEAMENTO::INICIA_CONTAGEM_COTIDIANO:                 return F("INICIA_CONTAGEM_COTIDIANO");  
            case BOMBEAMENTO::CANCELA_CONTAGEM_COTIDIANO:                return F("CANCELA_CONTAGEM_COTIDIANO");
            case BOMBEAMENTO::VERIFICA_CONTAGEM_COTIDIANO:               return F("VERIFICA_CONTAGEM_COTIDIANO"); 
            case BOMBEAMENTO::PREPARA_EXERCICIO_BOMBA:                   return F("PREPARA_EXERCICIO_BOMBA");  
            case BOMBEAMENTO::INICIO_EXERCICIO_BOMBA:                    return F("INICIO_EXERCICIO_BOMBA");   
            case BOMBEAMENTO::FIM_EXERCICIO_BOMBA:                       return F("FIM_EXERCICIO_BOMBA");  
            case BOMBEAMENTO::FINALIZADO:                                return F("FINALIZADO");  
            
            default:                return F("DESCONHECIDO");  
        }  
    }  
} 



// [FIM::BAT::PROTECAO_TERMICA_BOMBA_CASA]

// [INSERE::BAT::ESTADOS_CPP_Eco_Hidro] 