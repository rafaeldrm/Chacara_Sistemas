//====================================================================================                              
//                   DEPENDÊNCIAS                              
//====================================================================================                              
                                                       
#include "Estados_Sistema_Eco_Hidro.h"                              
#include "DEBUG_MODE.h"  
                              
//====================================================================================                              
//                   VARIÁVEIS INTERNAS (ESTÁTICAS)                              
//====================================================================================                              
                          
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
    static ESTADO estadoAtual = ESTADO::NORMAL;
    static ESTADO proximoEstado = ESTADO::NORMAL;
    static bool solicitacaoPendente = false; 

    void definir(ESTADO novo)
    {  
        // Em vez de mudar na hora, apenas agenda para o final/início do ciclo
        proximoEstado = novo;
        solicitacaoPendente = true;
    }

    // Função chamada controladamente no final ou início do loop principal
    void atualizarCiclo() {
        if (!solicitacaoPendente) return;
    
            ESTADO antigo = estadoAtual;
            estadoAtual = proximoEstado;
            solicitacaoPendente = false;
            
            // O log e o acionamento físico acontecem de forma síncrona e controlada aqui
            debugVariavel("CASA TORRE (anterior) : ", texto(antigo), LINHA, VERMELHO, LARANJA);
            debugVariavel("CASA TORRE : ", texto(estadoAtual), LINHA, VERDE, AMARELO);
    }
                 
    ESTADO atual()                    
    {                    
         return estadoAtual;                     
    }
  
    const __FlashStringHelper* texto(ESTADO a)    
    {    
        switch(a)    
        {    
            case ESTADO::NORMAL:                                      return F("NORMAL");   
            case ESTADO::PRIMEIRO_CICLO_ENCHIMENTO_CAIXA:             return F("PRIMEIRO_CICLO_ENCHIMENTO_CAIXA");   
            case ESTADO::AGUARDANDO_TRANSFERENCIA_GRAVIDADE:          return F("AGUARDANDO_TRANSFERENCIA_GRAVIDADE");   
            case ESTADO::SEGUNDO_CICLO_ENCHIMENTO_CAIXA:              return F("SEGUNDO_CICLO_ENCHIMENTO_CAIXA");   
            case ESTADO::FINALIZADO:                                  return F("FINALIZADO");
            case ESTADO::LIBERADO_PARA_O_RESERVATORIO:                return F("LIBERADO_PARA_O_RESERVATORIO");
            case ESTADO::BLOQUEIO:                                    return F("BLOQUEIO");   
    
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
    static ESTADO estadoAtual = ESTADO::REPOUSO;
    static ESTADO proximoEstado = ESTADO::REPOUSO;
    static bool solicitacaoPendente = false;

    void definir(ESTADO novo)
    {
        // Em vez de mudar na hora, apenas agenda para o final/início do ciclo
        proximoEstado = novo;
        solicitacaoPendente = true;
    }

    // Função chamada controladamente no final ou início do loop principal
    void atualizarCiclo() {
        if (!solicitacaoPendente) return;
        
            ESTADO antigo = estadoAtual;
            estadoAtual = proximoEstado;
            solicitacaoPendente = false;
            
            // O log e o acionamento físico acontecem de forma síncrona e controlada aqui
            debugVariavel("BOMBA CASA (anterior) : ", texto(antigo), LINHA, VERMELHO, LARANJA);
            debugVariavel("BOMBA CASA : ", texto(estadoAtual), LINHA, VERDE, AMARELO);
    }
                 
    ESTADO atual()                    
    {                    
         return estadoAtual;                     
    }
      
    const __FlashStringHelper* texto(ESTADO a)      
    {      
        switch(a)      
        {      
            case ESTADO::REPOUSO:                                   return F("REPOUSO");     
            case ESTADO::LIBERADO:                                  return F("LIBERADO");     
            case ESTADO::AGUARDA_LIBERACAO:                         return F("AGUARDA_LIBERACAO");     
            case ESTADO::LIGANDO_BOMBA:                             return F("LIGANDO_BOMBA");     
            case ESTADO::BOMBA_LIGADA:                              return F("BOMBA_LIGADA");     
            case ESTADO::DESLIGANDO_BOMBA:                          return F("DESLIGANDO_BOMBA");
            case ESTADO::BLOQUEIO:                                  return F("BLOQUEIO");
            case ESTADO::BLOQUEADO:                                 return F("BLOQUEADO");      
      
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
    static ESTADO estadoAtual = ESTADO::NORMAL;
    static ESTADO proximoEstado = ESTADO::NORMAL;
    static bool solicitacaoPendente = false; 

    void definir(ESTADO novo)
    {        
        // Em vez de mudar na hora, apenas agenda para o final/início do ciclo
        proximoEstado = novo;
        solicitacaoPendente = true;
    }

    // Função chamada controladamente no final ou início do loop principal
    void atualizarCiclo() {
        if (!solicitacaoPendente) return;
        
            ESTADO antigo = estadoAtual;
            estadoAtual = proximoEstado;
            solicitacaoPendente = false;
            
            // O log e o acionamento físico acontecem de forma síncrona e controlada aqui
            debugVariavel("RESERVATORIO (anterior) : ", texto(antigo), LINHA, VERMELHO, LARANJA);
            debugVariavel("RESERVATORIO : ", texto(estadoAtual), LINHA, VERDE, AMARELO);
    }
                 
    ESTADO atual()                    
    {                    
         return estadoAtual;                     
    }                         
                           
    const __FlashStringHelper* texto(ESTADO a)                           
    {                           
        switch(a)                           
        {                           
            case ESTADO::NORMAL:                               return F("NORMAL");                           
            case ESTADO::LIBERADO:                             return F("LIBERADO");
            case ESTADO::TRANSFERENCIA_RESERVATORIOS:          return F("TRANSFERENCIA_RESERVATORIOS");         
            case ESTADO::SO_MAIS_UM_POUQUINHO:                 return F("SO_MAIS_UM_POUQUINHO");   
            case ESTADO::FINALIZADO:                           return F("FINALIZADO");                                
            case ESTADO::BLOQUEIO_SEGURANCA:                   return F("BLOQUEIO_SEGURANCA");                           
                           
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
    static ESTADO estadoAtual = ESTADO::REPOUSO;
    static ESTADO proximoEstado = ESTADO::REPOUSO;
    static bool solicitacaoPendente = false; 

    void definir(ESTADO novo)
    {
        // Em vez de mudar na hora, apenas agenda para o final/início do ciclo
        proximoEstado = novo;
        solicitacaoPendente = true;
    }

    // Função chamada controladamente no final ou início do loop principal
    void atualizarCiclo() {
        if (!solicitacaoPendente) return;
        
            ESTADO antigo = estadoAtual;
            estadoAtual = proximoEstado;
            solicitacaoPendente = false;
            
            // O log e o acionamento físico acontecem de forma síncrona e controlada aqui
            debugVariavel("BOMBA_12V (anterior) : ", texto(antigo), LINHA, VERMELHO, LARANJA);
            debugVariavel("BOMBA 12V : ", texto(estadoAtual), LINHA, VERDE, AMARELO);
    }
                 
    ESTADO atual()                    
    {                    
         return estadoAtual;                     
    }       
         
    const __FlashStringHelper* texto(ESTADO a)         
    {         
        switch(a)         
        {         
            case ESTADO::REPOUSO:                                   return F("REPOUSO");        
            case ESTADO::LIBERADO:                                  return F("LIBERADO");        
            case ESTADO::AGUARDA_LIBERACAO:                        return F("AGUARDA_LIBERACAO");           
            case ESTADO::LIGANDO_BOMBA:                             return F("LIGANDO_BOMBA");        
            case ESTADO::BOMBA_LIGADA:                              return F("BOMBA_LIGADA");        
            case ESTADO::DESLIGANDO_BOMBA:                          return F("DESLIGANDO_BOMBA");           
         
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
    static ESTADO estadoAtual = ESTADO::NORMAL;
    static ESTADO proximoEstado = ESTADO::NORMAL;
    static bool solicitacaoPendente = false; 

    void definir(ESTADO novo)
    {
        
        // Em vez de mudar na hora, apenas agenda para o final/início do ciclo
        proximoEstado = novo;
        solicitacaoPendente = true;
    }

    // Função chamada controladamente no final ou início do loop principal
    void atualizarCiclo() {
        if (!solicitacaoPendente) return;
        
            ESTADO antigo = estadoAtual;
            estadoAtual = proximoEstado;
            solicitacaoPendente = false;
            
            // O log e o acionamento físico acontecem de forma síncrona e controlada aqui
            debugVariavel("SISTEMA ECO HIDRO (anterior) : ", texto(antigo), LINHA, VERMELHO, LARANJA);
            debugVariavel("SISTEMA ECO HIDRO : ", texto(estadoAtual), LINHA, VERDE, AMARELO);
        }

    ESTADO atual()                    
    {                    
         return estadoAtual;                     
    }                
                 
    const __FlashStringHelper* texto(ESTADO a)                 
    {                 
        switch(a)                 
        {                 
            case ESTADO::NORMAL:                                        return F("NORMAL");                
            case ESTADO::CICLO_COMPLETO:                                return F("CICLO_COMPLETO");                
            case ESTADO::MEIO_CICLO:                                    return F("MEIO_CICLO");                
            case ESTADO::BLOQUEIO:                                      return F("BLOQUEIO");                
            case ESTADO::CONTADOR_PULSO:                                return F("CONTADOR_PULSO");                
                 
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
    static ESTADO estadoAtual = ESTADO::DESLIGADO;
    static ESTADO proximoEstado = ESTADO::DESLIGADO;
    static bool solicitacaoPendente = false; 

    void definir(ESTADO novo)
    {
        
        // Em vez de mudar na hora, apenas agenda para o final/início do ciclo
        proximoEstado = novo;
        solicitacaoPendente = true;
    }

    // Função chamada controladamente no final ou início do loop principal
    void atualizarCiclo() {
        if (!solicitacaoPendente) return;
        
      
            ESTADO antigo = estadoAtual;
            estadoAtual = proximoEstado;
            solicitacaoPendente = false;
            
            // O log e o acionamento físico acontecem de forma síncrona e controlada aqui
            debugVariavel("LUZ ESTACIONAMENTO (anterior) : ", texto(antigo), LINHA, VERMELHO, LARANJA);
            debugVariavel("LUZ ESTACIONAMENTO : ", texto(estadoAtual), LINHA, VERDE, AMARELO);
    }
                 
    ESTADO atual()                    
    {                    
         return estadoAtual;                     
    }                
                        
    const __FlashStringHelper* texto(ESTADO a)                        
    {                        
        switch(a)                        
        {                        
            case ESTADO::DESLIGADO:                                 return F("DESLIGADO");     
            case ESTADO::DESLIGANDO:                                return F("DESLIGANDO");
            case ESTADO::INICIANDO_TEMPORIZACAO:                    return F("INICIANDO_TEMPORIZACAO");                     
            case ESTADO::TEMPORIZADO:                               return F("TEMPORIZADO");                        
            case ESTADO::FORCADO:                                   return F("FORCADO");   
            case ESTADO::CONSUMINDO:                                return F("CONSUMINDO");
            case ESTADO::ACIONADO:                                  return F("ACIONADO");                        
                        
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
    static ESTADO estadoAtual = ESTADO::DESLIGADO;
    static ESTADO proximoEstado = ESTADO::DESLIGADO;
    static bool solicitacaoPendente = false;

    void definir(ESTADO novo)
    {
        
        
        // Em vez de mudar na hora, apenas agenda para o final/início do ciclo
        proximoEstado = novo;
        solicitacaoPendente = true;
    }

    // Função chamada controladamente no final ou início do loop principal
    void atualizarCiclo() {
        if (!solicitacaoPendente) return;
        
      
            ESTADO antigo = estadoAtual;
            estadoAtual = proximoEstado;
            solicitacaoPendente = false;

            // O log e o acionamento físico acontecem de forma síncrona e controlada aqui
             debugVariavel("LUZ ACESSO RESERVATÓRIO (anterior) : ", texto(antigo), LINHA, VERMELHO, LARANJA);
            debugVariavel("LUZ ACESSO RESERVATÓRIO : ", texto(estadoAtual), LINHA, VERDE, AMARELO);

    }
                 
    ESTADO atual()                    
    {                    
         return estadoAtual;                     
    }            
                    
    const __FlashStringHelper* texto(ESTADO a)                    
    {                    
        switch(a)                    
        {                    
            case ESTADO::DESLIGADO:                                 return F("DESLIGADO");     
            case ESTADO::DESLIGANDO:                                return F("DESLIGANDO");
            case ESTADO::INICIANDO_TEMPORIZACAO:                    return F("INICIANDO_TEMPORIZACAO");                 
            case ESTADO::TEMPORIZADO:                               return F("TEMPORIZADO");   
            case ESTADO::FORCADO:                                   return F("FORCADO");   
            case ESTADO::CONSUMINDO:                                return F("CONSUMINDO");
            case ESTADO::ACIONADO:                                  return F("ACIONADO");              
                    
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
    static ESTADO estadoAtual = ESTADO::NORMAL;
    static ESTADO proximoEstado = ESTADO::NORMAL;
    static bool solicitacaoPendente = false;

    void definir(ESTADO novo)
    {
        
        
        // Em vez de mudar na hora, apenas agenda para o final/início do ciclo
        proximoEstado = novo;
        solicitacaoPendente = true;
    }

    // Função chamada controladamente no final ou início do loop principal
    void atualizarCiclo() {
        if (!solicitacaoPendente) return;
        
      
            ESTADO antigo = estadoAtual;
            estadoAtual = proximoEstado;
            solicitacaoPendente = false;
            
            // O log e o acionamento físico acontecem de forma síncrona e controlada aqui
             debugVariavel("VOLTAGEM (anterior) : ", texto(antigo), LINHA, VERMELHO, LARANJA);
            debugVariavel("VOLTAGEM : ", texto(estadoAtual), LINHA, VERDE, AMARELO);
        }
                 
    ESTADO atual()                    
    {                    
         return estadoAtual;                     
    }                   
                      
    const __FlashStringHelper* texto(ESTADO a)                      
    {                      
        switch(a)                      
        {                      
            case ESTADO::NORMAL:                                     return F("NORMAL");                      
            case ESTADO::IDEAL:                                      return F("IDEAL");                      
            case ESTADO::CHEIA:                                      return F("CHEIA");   
            case ESTADO::MANTENDO_CARGA:                             return F("MANTENDO_CARGA");    
            case ESTADO::CARGA_CONSUMIDA:                            return F("CARGA_CONSUMIDA");                     
            case ESTADO::CRITICO:                                    return F("CRITICO"); 
            case ESTADO::CARGA_PRIORIDADE:              return F("CARGA_PRIORIDADE");                     
                      
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
    static ESTADO estadoAtual = ESTADO::NENHUM;
    static ESTADO proximoEstado = ESTADO::NENHUM;
    static bool solicitacaoPendente = false; 

    void definir(ESTADO novo)
    {
        
        
        // Em vez de mudar na hora, apenas agenda para o final/início do ciclo
        proximoEstado = novo;
        solicitacaoPendente = true;
    }

    // Função chamada controladamente no final ou início do loop principal
    void atualizarCiclo() {
        if (!solicitacaoPendente) return;
        
      
            ESTADO antigo = estadoAtual;
            estadoAtual = proximoEstado;
            solicitacaoPendente = false;  

            // O log e o acionamento físico acontecem de forma síncrona e controlada aqui
             debugVariavel("DESLGAMENTO BOMBA (anterior) : ", texto(antigo), LINHA, VERMELHO, LARANJA);
            debugVariavel("DESLIGAMENTO BOMBA : ", texto(estadoAtual), LINHA, VERDE, AMARELO);
        }
        
    ESTADO atual()                    
    {                    
         return estadoAtual;                     
    }       
          
    const __FlashStringHelper* texto(ESTADO a)          
    {          
        switch(a)          
        {          
            case ESTADO::NENHUM:                                      return F("NENHUM");         
            case ESTADO::INICIADO_BOMBA_CASA:                         return F("INICIADO_BOMBA_CASA");         
            case ESTADO::BOMBA_OK:                                    return F("BOMBA_OK");         
            case ESTADO::CASA_MINIMA:                                 return F("CASA_MINIMA");         
            case ESTADO::TORRE_MAXIMA:                                return F("TORRE_MAXIMA");         
            case ESTADO::ESTOQUE_MINIMO:                              return F("ESTOQUE_MINIMO");         
            case ESTADO::DHT11_TEMPERATURA:                           return F("DHT11_TEMPERATURA");   
            case ESTADO::DHT11_ERRO:                                  return F("DHT11_ERRO");        
            case ESTADO::SENSOR_IR_BOMBA_TRAVADO:                     return F("SENSOR_IR_BOMBA_TRAVADO");         
            case ESTADO::TEMPO_MAXIMO:                                return F("TEMPO_MAXIMO");         
            case ESTADO::RESIDENCIAL_ORDENOU:                         return F("RESIDENCIAL_ORDENOU");         
          
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
    static ESTADO estadoAtual = ESTADO::REPOUSO;
    static ESTADO proximoEstado = ESTADO::REPOUSO;
    static bool solicitacaoPendente = false; 

    void definir(ESTADO novo)
    {
        
        
        // Em vez de mudar na hora, apenas agenda para o final/início do ciclo
        proximoEstado = novo;
        solicitacaoPendente = true;
    }

    // Função chamada controladamente no final ou início do loop principal
    void atualizarCiclo() {
        if (!solicitacaoPendente) return;
        
      
            ESTADO antigo = estadoAtual;
            estadoAtual = proximoEstado;
            solicitacaoPendente = false;
            
            // O log e o acionamento físico acontecem de forma síncrona e controlada aqui
            debugVariavel("PORTAO (anterior) : ", texto(antigo), LINHA, VERMELHO, LARANJA);
            debugVariavel("PORTAO : ", texto(estadoAtual), LINHA, VERDE, AMARELO);
        }
           
    ESTADO atual()                    
    {                    
         return estadoAtual;                     
    }            
    const __FlashStringHelper* texto(ESTADO a)                    
    {                    
        switch(a)                    
        {                    
            case ESTADO::REPOUSO:                                   return F("REPOUSO");                    
            case ESTADO::LIBERADO:                                  return F("LIBERADO");                    
            case ESTADO::AGUARDA_LIBERACAO:                         return F("AGUARDA_LIBERACAO");                    
            case ESTADO::LIGANDO_PORTAO:                            return F("LIGANDO_PORTAO");
            case ESTADO::PORTAO_LIGADO:                             return F("PORTAO_LIGADO");
            case ESTADO::DESLIGANDO_PORTAO:                         return F("DESLIGANDO_PORTAO");                    
                    
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
    static ESTADO estadoAtual = ESTADO::REPOUSO;
    static ESTADO proximoEstado = ESTADO::REPOUSO;
    static bool solicitacaoPendente = false;

    void definir(ESTADO novo)
    {
        
        
        // Em vez de mudar na hora, apenas agenda para o final/início do ciclo
        proximoEstado = novo;
        solicitacaoPendente = true;
    }

    // Função chamada controladamente no final ou início do loop principal
    void atualizarCiclo() {
        if (!solicitacaoPendente) return;
        
      
            ESTADO antigo = estadoAtual;
            estadoAtual = proximoEstado;
            solicitacaoPendente = false;
            
            // O log e o acionamento físico acontecem de forma síncrona e controlada aqui
            debugVariavel("UNIDADE POTENCIA (anterior) : ", texto(antigo), LINHA, VERMELHO, LARANJA);
            debugVariavel("UNIDADE POTENCIA : ", texto(estadoAtual), LINHA, VERDE, AMARELO);
        }
          
    ESTADO atual()                    
    {                    
         return estadoAtual;                     
    }
   
    const __FlashStringHelper* texto(ESTADO a)   
    {   
        switch(a)   
        {   
            case ESTADO::REPOUSO:                                     return F("REPOUSO");   
            case ESTADO::ACIONAMENTO_RELES:                           return F("ACIONAMENTO_RELES");   
            case ESTADO::AGUARDANDO_ESTABILIZAR:                      return F("AGUARDANDO_ESTABILIZAR");   
            case ESTADO::LIGADO:                                      return F("LIGADO");   
            case ESTADO::DESLIGANDO:                                  return F("DESLIGANDO");   
   
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
    static ESTADO estadoAtual = ESTADO::REPOUSO;
    static ESTADO proximoEstado = ESTADO::REPOUSO;
    static bool solicitacaoPendente = false;

    void definir(ESTADO novo)
    {
        
        
        // Em vez de mudar na hora, apenas agenda para o final/início do ciclo
        proximoEstado = novo;
        solicitacaoPendente = true;
    }

    // Função chamada controladamente no final ou início do loop principal
    void atualizarCiclo() {
        if (!solicitacaoPendente) return;
        
      
            ESTADO antigo = estadoAtual;
            estadoAtual = proximoEstado;
            solicitacaoPendente = false;
            
            // O log e o acionamento físico acontecem de forma síncrona e controlada aqui
            debugVariavel("POTENCIA (anterior) : ", texto(antigo), LINHA, VERMELHO, LARANJA);
            debugVariavel("POTENCIA : ", texto(estadoAtual), LINHA, VERDE, AMARELO);
        }
                 
    ESTADO atual()                    
    {                    
         return estadoAtual;                     
    }  
   
    const __FlashStringHelper* texto(ESTADO a)   
    {   
        switch(a)   
        {   
            case ESTADO::REPOUSO:                                   return F("REPOUSO");   
            case ESTADO::BOMBA_CASA:                                return F("BOMBA_CASA");   
            case ESTADO::BOMBA_12V:                                 return F("BOMBA_12V");   
            case ESTADO::PORTAO:                                    return F("PORTAO");   
            case ESTADO::DESLIGANDO:                                return F("DESLIGANDO");   
   
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
    static ESTADO estadoAtual = ESTADO::REPOUSO;
    static ESTADO proximoEstado = ESTADO::REPOUSO;
    static bool solicitacaoPendente = false; 

    void definir(ESTADO novo)
    {
        
        
        // Em vez de mudar na hora, apenas agenda para o final/início do ciclo
        proximoEstado = novo;
        solicitacaoPendente = true;
    }

    // Função chamada controladamente no final ou início do loop principal
    void atualizarCiclo() {
        if (!solicitacaoPendente) return;
        
      
            ESTADO antigo = estadoAtual;
            estadoAtual = proximoEstado;
            solicitacaoPendente = false;
            
            // O log e o acionamento físico acontecem de forma síncrona e controlada aqui
            debugVariavel("TRANCO BOMBA (anterior) : ", texto(antigo), LINHA, VERMELHO, LARANJA);
            debugVariavel("TRANCO BOMBA : ", texto(estadoAtual), LINHA, VERDE, AMARELO);
        }
                  
    ESTADO atual()                    
    {                    
         return estadoAtual;                     
    }
  
    const __FlashStringHelper* texto(ESTADO a)  
    {  
        switch(a)  
        {  
            case ESTADO::REPOUSO:                                   return F("REPOUSO");  
            case ESTADO::INICIANDO_MONITORAMENTO_HELICE:            return F("INICIANDO_MONITORAMENTO_HELICE");  
            case ESTADO::BOMBA_TRAVADA:                             return F("BOMBA_TRAVADA");  
            case ESTADO::PRIMEIRA_TENTATIVA_TRANCO:                 return F("PRIMEIRA_TENTATIVA_TRANCO");  
            case ESTADO::SEGUNDA_TENTATIVA_TRANCO:                  return F("SEGUNDA_TENTATIVA_TRANCO");   
            case ESTADO::SENSOR_IR_DESATIVADO:                      return F("SENSOR_IR_DESATIVADO");  
            case ESTADO::BOMBA_LIBERADA:                            return F("BOMBA_LIBERADA");
  
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
    static ESTADO estadoAtual = ESTADO::REPOUSO;
    static ESTADO proximoEstado = ESTADO::REPOUSO;
    static bool solicitacaoPendente = false; 

    void definir(ESTADO novo)
    {
        
        
        // Em vez de mudar na hora, apenas agenda para o final/início do ciclo
        proximoEstado = novo;
        solicitacaoPendente = true;
    }

    // Função chamada controladamente no final ou início do loop principal
    void atualizarCiclo() {
        if (!solicitacaoPendente) return;
        
      
            ESTADO antigo = estadoAtual;
            estadoAtual = proximoEstado;
            solicitacaoPendente = false;
            
            // O log e o acionamento físico acontecem de forma síncrona e controlada aqui
            debugVariavel("PROTECAO TERMICA BOMBA (anterior) : ", texto(antigo), LINHA, VERMELHO, LARANJA);
            debugVariavel("PROTECAO TERMICA BOMBA : ", texto(estadoAtual), LINHA, VERDE, AMARELO);
        }
                  
    ESTADO atual()                    
    {                    
         return estadoAtual;                     
    }
  
    const __FlashStringHelper* texto(ESTADO a)  
    {  
        switch(a)  
        {  
            case ESTADO::REPOUSO:                                   return F("REPOUSO");  
            case ESTADO::MONITORAMENTO_ATIVADO:                     return F("MONITORAMENTO_ATIVADO");  
            case ESTADO::SUPERAQUECIMENTO:                          return F("SUPERAQUECIMENTO");  
            case ESTADO::MODO_SEGURANCA_PRIMEIRO_CICLO:             return F("MODO_SEGURANCA_PRIMEIRO_CICLO");  
            case ESTADO::MODO_SEGURANCA_SEGUNDO_CICLO:              return F("MODO_SEGURANCA_SEGUNDO_CICLO");   
            case ESTADO::FINALIZADO:                                return F("FINALIZADO");  
            
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
    static ESTADO estadoAtual = ESTADO::REPOUSO;
    static ESTADO proximoEstado = ESTADO::REPOUSO;
    static bool solicitacaoPendente = false; 

    void definir(ESTADO novo)
    {
        
        
        // Em vez de mudar na hora, apenas agenda para o final/início do ciclo
        proximoEstado = novo;
        solicitacaoPendente = true;
    }

    // Função chamada controladamente no final ou início do loop principal
    void atualizarCiclo() {
        if (!solicitacaoPendente) return;
        
      
            ESTADO antigo = estadoAtual;
            estadoAtual = proximoEstado;
            solicitacaoPendente = false;
            
            // O log e o acionamento físico acontecem de forma síncrona e controlada aqui
            debugVariavel("GIRO PREVENTIVO BOMBA CASA (anterior) : ", texto(antigo), LINHA, VERMELHO, LARANJA);
            debugVariavel("GIRO PREVENTIVO BOMBA CASA : ", texto(estadoAtual), LINHA, VERDE, AMARELO);
        }
                 
    ESTADO atual()                    
    {                    
         return estadoAtual;                     
    }
  
    const __FlashStringHelper* texto(ESTADO a)  
    {  
        switch(a)  
        {  
            case ESTADO::REPOUSO:                                    return F("REPOUSO");
            case ESTADO::INICIA_CONTAGEM_COTIDIANO:                 return F("INICIA_CONTAGEM_COTIDIANO");  
            case ESTADO::CANCELA_CONTAGEM_COTIDIANO:                return F("CANCELA_CONTAGEM_COTIDIANO");
            case ESTADO::VERIFICA_CONTAGEM_COTIDIANO:               return F("VERIFICA_CONTAGEM_COTIDIANO"); 
            case ESTADO::PREPARA_EXERCICIO_BOMBA:                   return F("PREPARA_EXERCICIO_BOMBA");  
            case ESTADO::INICIO_EXERCICIO_BOMBA:                    return F("INICIO_EXERCICIO_BOMBA");   
            case ESTADO::FIM_EXERCICIO_BOMBA:                       return F("FIM_EXERCICIO_BOMBA");  
            case ESTADO::FINALIZADO:                                return F("FINALIZADO");  
            
            default:                return F("DESCONHECIDO");  
        }  
    }  
} 



// [FIM::BAT::PROTECAO_TERMICA_BOMBA_CASA]

// [INSERE::BAT::ESTADOS_CPP_Eco_Hidro] 