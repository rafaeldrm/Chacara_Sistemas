//====================================================================================                              
//                   DEPENDÊNCIAS                              
//====================================================================================                              
                                                       
#include "Estados_Sistema_Gate_Drimel.h"                              
#include "DEBUG_MODE.h"  
                              
//====================================================================================                              
//                   VARIÁVEIS INTERNAS (ESTÁTICAS)                              
//====================================================================================                              
                                                     
//====================================================================================                              
//                   IMPLEMENTAÇÃO DOS NAMESPACES                              
//====================================================================================                               
    
//====================================================================================    
//                   POSICIONAMENTO PORTAO    
//====================================================================================    
    
namespace POSICIONAMENTO_PORTAO    
{    
    // Estado atual real e o próximo estado desejado para o próximo tick
    static ESTADO estadoAtual     = ESTADO::DESCONHECIDO;
    static ESTADO proximoEstado   = ESTADO::DESCONHECIDO;
    static bool solicitacaoPendente = false; 

    void definir(ESTADO novo)
    {
    //    
    
        // Em vez de mudar na hora, apenas agenda para o final/início do ciclo
        proximoEstado = novo;
        solicitacaoPendente = (novo != estadoAtual);
    }

    // Função chamada controladamente no final ou início do loop principal
    void atualizarCiclo() {
        if (!solicitacaoPendente) return;
        
      //
            ESTADO antigo = estadoAtual;
            estadoAtual = proximoEstado;
            solicitacaoPendente = false;
            
            // O log e o acionamento físico acontecem de forma síncrona e controlada aqui
            debugVariavel("POSICIONAMENTO PORTAO (anterior) : ", texto(antigo), LINHA, VERMELHO, LARANJA);
            debugVariavel("POSICIONAMENTO PORTAO : ", texto(estadoAtual), LINHA, VERDE, AMARELO);
     //   }
    }
                 
    ESTADO atual()                    
    {                    
         return estadoAtual;                     
    }
  
    const __FlashStringHelper* texto(ESTADO a)    
    {    
        switch(a)    
        {   
            case ESTADO::DESCONHECIDO:                                 return F("DESCONHECIDO");
            case ESTADO::ABRINDO:                                      return F("ABRINDO");
            case ESTADO::ABERTO:                                       return F("ABERTO");
            case ESTADO::FECHANDO:                                     return F("FECHANDO"); 
            case ESTADO::FECHADO:                                      return F("FECHADO");   
            
   
    
            default:                return F("INVALIDO");    
        }    
    }    
}   


//====================================================================================    
//                   TRINCO PORTAO    
//====================================================================================    
    
namespace TRINCO_PORTAO    
{    
    // Estado atual real e o próximo estado desejado para o próximo tick
    static ESTADO estadoAtual = ESTADO::DESCONHECIDO;
    static ESTADO proximoEstado = ESTADO::DESCONHECIDO;
    static bool solicitacaoPendente = false; 

    void definir(ESTADO novo)
    {
        
        // Em vez de mudar na hora, apenas agenda para o final/início do ciclo
         proximoEstado = novo;
        solicitacaoPendente = (novo != estadoAtual);
    }

    // Função chamada controladamente no final ou início do loop principal
    void atualizarCiclo() {
        if (!solicitacaoPendente) return;
        
      
            ESTADO antigo = estadoAtual;
            estadoAtual = proximoEstado;
            solicitacaoPendente = false;
            
            // O log e o acionamento físico acontecem de forma síncrona e controlada aqui
            debugVariavel("TRINCO PORTAO (anterior) : ", texto(antigo), LINHA, VERMELHO, LARANJA);
            debugVariavel("TRINCO PORTAO : ", texto(estadoAtual), LINHA, VERDE, AMARELO);
        }
                 
    ESTADO atual()                    
    {                    
         return estadoAtual;                     
    }
  
    const __FlashStringHelper* texto(ESTADO a)    
    {    
        switch(a)    
        {   
            case ESTADO::DESCONHECIDO:                                     return F("DESCONHECIDO");
            case ESTADO::SINALIZADO_COMO_ABERTO:                           return F("SINALIZADO_COMO_ABERTO");
            case ESTADO::SINALIZADO_COMO_FECHADO:                          return F("SINALIZADO_COM_FECHADO");
            case ESTADO::FALHA_SENSOR:                                     return F("FALHA_SENSOR"); 
    
            default:                return F("DESCONHECIDO");    
        }    
    }    
}    
    

//====================================================================================    
//                   TRANCA MAGNÉTICA    
//====================================================================================    
    
namespace TRANCA_MAGNETICA    
{    
    // Estado atual real e o próximo estado desejado para o próximo tick
    static ESTADO estadoAtual = ESTADO::TRANCADO;
    static ESTADO proximoEstado = ESTADO::SOLTO;
    static bool solicitacaoPendente = false; 

    void definir(ESTADO novo)
    {
    
        // Em vez de mudar na hora, apenas agenda para o final/início do ciclo
         proximoEstado = novo;
        solicitacaoPendente = (novo != estadoAtual);
    }

    // Função chamada controladamente no final ou início do loop principal
    void atualizarCiclo() {
        if (!solicitacaoPendente) return;
        
      
            ESTADO antigo = estadoAtual;
            estadoAtual = proximoEstado;
            solicitacaoPendente = false;
            
            // O log e o acionamento físico acontecem de forma síncrona e controlada aqui
            debugVariavel("TRANCA MAGNETICA (anterior) : ", texto(antigo), LINHA, VERMELHO, LARANJA);
            debugVariavel("TRANCA MAGNETICA : ", texto(estadoAtual), LINHA, VERDE, AMARELO);
        }
                 
    ESTADO atual()                    
    {                    
         return estadoAtual;                     
    }
  
    const __FlashStringHelper* texto(ESTADO a)    
    {    
        switch(a)    
        {   
            case ESTADO::TRANCADO:                                   return F("TRANCADO");
            case ESTADO::SOLTO:                                      return F("SOLTO");
            case ESTADO::LENDO_VOLTAGEM:                             return F("LENDO_VOLTAGEM");
   
    
            default:                return F("DESCONHECIDO");    
        }    
    }    
}    
    

//====================================================================================    
//                   FORA CHÁCARA    
//====================================================================================    
    
namespace FORA_CHACARA   
{    
    // Estado atual real e o próximo estado desejado para o próximo tick
    static ESTADO estadoAtual = ESTADO::REPOUSO;
    static ESTADO proximoEstado = ESTADO::REPOUSO;
    static bool solicitacaoPendente = false; 

    void definir(ESTADO novo)
    {
        
    
        // Em vez de mudar na hora, apenas agenda para o final/início do ciclo
         proximoEstado = novo;
        solicitacaoPendente = (novo != estadoAtual);
    }

    // Função chamada controladamente no final ou início do loop principal
    void atualizarCiclo() {
        if (!solicitacaoPendente) return;
        
      
            ESTADO antigo = estadoAtual;
            estadoAtual = proximoEstado;
            solicitacaoPendente = false;
            
            // O log e o acionamento físico acontecem de forma síncrona e controlada aqui
            debugVariavel("FORA CHACARA (anterior) : ", texto(antigo), LINHA, VERMELHO, LARANJA);
            debugVariavel("FORA CHÁCARA : ", texto(estadoAtual), LINHA, VERDE, AMARELO);
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
            case ESTADO::LENDO:                                      return F("LENDO");
            case ESTADO::VEICULO_PRESENTE:                           return F("VEICULO_PRESENTE");
            case ESTADO::VEICULO_AUSENTE:                            return F("VEICULO_AUSENTE"); 
    
            default:                return F("DESCONHECIDO");    
        }    
    }    
}    
    
//====================================================================================    
//                   DENTRO CHÁCARA    
//====================================================================================    
    
namespace DENTRO_CHACARA   
{    
    // Estado atual real e o próximo estado desejado para o próximo tick
    static ESTADO estadoAtual = ESTADO::REPOUSO;
    static ESTADO proximoEstado = ESTADO::REPOUSO;
    static bool solicitacaoPendente = false; 

    void definir(ESTADO novo)
    {
        
    
        // Em vez de mudar na hora, apenas agenda para o final/início do ciclo
         proximoEstado = novo;
        solicitacaoPendente = (novo != estadoAtual);
    }

    // Função chamada controladamente no final ou início do loop principal
    void atualizarCiclo() {
        if (!solicitacaoPendente) return;
        
      
            ESTADO antigo = estadoAtual;
            estadoAtual = proximoEstado;
            solicitacaoPendente = false;
            
            // O log e o acionamento físico acontecem de forma síncrona e controlada aqui
            debugVariavel("DENTRO CHACARA(anterior) : ", texto(antigo), LINHA, VERMELHO, LARANJA);
            debugVariavel("DENTRO CHÁCARA : ", texto(estadoAtual), LINHA, VERDE, AMARELO);
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
            case ESTADO::LENDO:                                      return F("LENDO");
            case ESTADO::VEICULO_PRESENTE:                           return F("VEICULO_PRESENTE");
            case ESTADO::VEICULO_AUSENTE:                            return F("VEICULO_AUSENTE"); 
    
            default:                return F("DESCONHECIDO");    
        }    
    }    
}    
