//-------------------------- DEPENDÊNCIAS ----------------------------------//                              
                                
#pragma once                              
#include <Arduino.h>                              
                              
//-------------------------- ESTRUTURAS DE ESTADOS DO SISTEMA Eco_Hidro ----------------------------------//                              
     
// [INICIO::BAT::CASA_TORRE]    
     
//====================================================================================    
//                   CASA_TORRE    
//====================================================================================    
    
namespace CASA_TORRE    
{    
enum class ETAPAS : uint8_t    
 {    
       NORMAL,   
       PRIMEIRO_CICLO_ENCHIMENTO_CAIXA,   
       AGUARDANDO_TRANSFERENCIA_GRAVIDADE,   
       SEGUNDO_CICLO_ENCHIMENTO_CAIXA,   
       FINALIZADO,   
 };    
    
    void definir(ETAPAS novo);    
    ETAPAS atual();    
    const __FlashStringHelper* texto(ETAPAS s);    
        
}    
    
// [FIM::BAT::CASA_TORRE]    
   
// [INICIO::BAT::BOMBA_CASA]      
       
//====================================================================================      
//                   BOMBA_CASA      
//====================================================================================      
      
namespace BOMBA_CASA      
{      
enum class ESTAGIOS : uint8_t      
 {      
      REPOUSO,        
      LIBERADO,        
      AGUARDA_LIBERACAO,       
      LIGANDO_BOMBA,        
      BOMBA_LIGADA,        
      DESLIGANDO_BOMBA,      
 };      
      
    void definir(ESTAGIOS novo);      
    ESTAGIOS atual();      
    const __FlashStringHelper* texto(ESTAGIOS s);      
          
}      
      
// [FIM::BAT::BOMBA_CASA]      
                  
// [INICIO::BAT::RESERVATORIO]                           
                            
//====================================================================================                           
//                   RESERVATORIO                           
//====================================================================================                           
                           
namespace RESERVATORIO                           
{                           
enum class TRANSFERENCIA : uint8_t                           
 {                           
       NORMAL,                           
       LIBERADO,                           
       SO_MAIS_UM_POUQUINHO,    
       FINALIZADO,                          
       BLOQUEIO_SEGURANCA,                           
 };                           
                           
    void definir(TRANSFERENCIA novo);                           
    TRANSFERENCIA atual();                           
    const __FlashStringHelper* texto(TRANSFERENCIA s);                           
                               
}                           
                           
// [FIM::BAT::RESERVATORIO]                           
                            
// [INICIO::BAT::BOMBA_12V]         
          
//====================================================================================         
//                   BOMBA_12V         
//====================================================================================         
         
namespace BOMBA_12V         
{         
enum class ESTAGIOS : uint8_t         
 {         
      REPOUSO,        
      LIBERADO,        
      AGUARDA_LIBERACAO,       
      LIGANDO_BOMBA,        
      BOMBA_LIGADA,        
      DESLIGANDO_BOMBA,             
 };         
         
    void definir(ESTAGIOS novo);         
    ESTAGIOS atual();         
    const __FlashStringHelper* texto(ESTAGIOS s);         
             
}         
         
// [FIM::BAT::BOMBA_12V]         
                 
// [INICIO::BAT::SISTEMA]                 
                  
//====================================================================================                 
//                   SISTEMA                 
//====================================================================================                 
                 
namespace SISTEMA_ECO_HIDRO                 
{                 
enum class MODO : uint8_t                 
 {                 
       NORMAL,                
       CICLO_COMPLETO,                
       MEIO_CICLO,                
       BLOQUEIO,                
       CONTADOR_PULSO,                
 };                 
                 
    void definir(MODO novo);                 
    MODO atual();                 
    const __FlashStringHelper* texto(MODO s);                 
                     
}                 
                 
// [FIM::BAT::SISTEMA]                              
                         
// [INICIO::BAT::LUZ_ESTACIONAMENTO]                        
                         
//====================================================================================                        
//                   LUZ_ESTACIONAMENTO                        
//====================================================================================                        
                        
namespace LUZ_ESTACIONAMENTO                        
{                        
enum class SITUACAO : uint8_t                        
 {                        
       DESLIGADO,   
       DESLIGANDO,                     
       TEMPORIZADO,                        
       FORCADO,   
       CONSUMINDO,   
   
 };                        
                        
    void definir(SITUACAO novo);                        
    SITUACAO atual();                        
    const __FlashStringHelper* texto(SITUACAO s);                        
                            
}                        
                        
// [FIM::BAT::LUZ_ESTACIONAMENTO]                        
              
// [INICIO::BAT::LUZ_ACESSO]                    
                     
//====================================================================================                    
//                   LUZ_ACESSO                    
//====================================================================================                    
                    
namespace LUZ_ACESSO                    
{                    
enum class SITUACAO : uint8_t                    
 {                    
       DESLIGADO,   
       DESLIGANDO,                   
       TEMPORIZADO,   
       FORCADO,   
       CONSUMINDO,   
 };                    
                    
    void definir(SITUACAO novo);                    
    SITUACAO atual();                    
    const __FlashStringHelper* texto(SITUACAO s);                    
                        
}                    
                    
// [FIM::BAT::LUZ_ACESSO]                    
                  
// [INICIO::BAT::VOLTAGEM]                      
                       
//====================================================================================                      
//                   VOLTAGEM                      
//====================================================================================                      
                      
namespace VOLTAGEM                      
{                      
enum class LEITURA : uint8_t                      
 {                      
       NORMAL,                      
       IDEAL,                      
       CHEIA,   
       MANTENDO_CARGA,   
       CARGA_CONSUMIDA,                   
       CRITICO,                      
 };                      
                      
    void definir(LEITURA novo);                      
    LEITURA atual();                      
    const __FlashStringHelper* texto(LEITURA s);                      
                          
}                      
                      
// [FIM::BAT::VOLTAGEM]                      
        
// [INICIO::BAT::DESLIGAMENTO_BOMBA]          
           
//====================================================================================          
//                   DESLIGAMENTO_BOMBA          
//====================================================================================          
          
namespace DESLIGAMENTO_BOMBA          
{          
enum class MOTIVO : uint8_t          
 {          
       NENHUM,         
       INICIADO_BOMBA_CASA,         
       BOMBA_OK,         
       CASA_MINIMA,         
       TORRE_MAXIMA,         
       ESTOQUE_MINIMO,         
       DHT11_TEMPERATURA,   
       DHT11_ERRO,        
       SENSOR_IR_BOMBA_TRAVADO,         
       TEMPO_MAXIMO,         
       RESIDENCIAL_ORDENOU,   //não utilizado ainda      
 };          
          
    void definir(MOTIVO novo);          
    MOTIVO atual();          
    const __FlashStringHelper* texto(MOTIVO s);          
              
}          
          
// [FIM::BAT::DESLIGAMENTO_BOMBA]          
                 
// [INICIO::BAT::PORTAO_ELETRONICO]                    
                     
//====================================================================================                    
//                   PORTAO_ELETRONICO                    
//====================================================================================                    
                    
namespace PORTAO_ELETRONICO                    
{                    
enum class ESTAGIOS : uint8_t                    
 {                    
      REPOUSO,        
      LIBERADO,        
      AGUARDA_LIBERACAO,       
      PORTAO_LIGADO,        
      PORTAO_ATIVADO,        
      DESLIGANDO_PORTAO,                     
 };                    
                    
    void definir(ESTAGIOS novo);                    
    ESTAGIOS atual();                    
    const __FlashStringHelper* texto(ESTAGIOS s);                    
                        
}                    
                    
// [FIM::BAT::PORTAO_ELETRONICO]                    
   
// [INICIO::BAT::UNIDADE_POTENCIA]   
   
//====================================================================================   
//                   UNIDADE_POTENCIA (Controle Central do Inversor)   
//====================================================================================   
   
namespace UNIDADE_POTENCIA   
{   
enum class ESTAGIOS : uint8_t   
 {   
       REPOUSO,   
       ACIONAMENTO_RELES,   
       AGUARDANDO_ESTABILIZAR,   
       LIGADO,   
       DESLIGANDO,   
 };   
   
    void definir(ESTAGIOS novo);   
    ESTAGIOS atual();   
    const __FlashStringHelper* texto(ESTAGIOS s);   
   
}   
   
// [FIM::BAT::UNIDADE_POTENCIA]   
   
// [INICIO::BAT::POTENCIA]   
   
//====================================================================================   
//                   POTENCIA (Qual está atualmente em uso)   
//====================================================================================   
   
namespace POTENCIA   
{   
enum class SELECIONADO : uint8_t   
      {   
            REPOUSO,   
            BOMBA_CASA,   
            BOMBA_12V,   
            PORTAO,   
            DESLIGANDO,   
      };   
         
      void definir(SELECIONADO novo);   
      SELECIONADO atual();   
      const __FlashStringHelper* texto(SELECIONADO s);   
         
      }   
   
// [FIM::BAT::POTENCIA]      
   
// [INICIO::BAT::TRANCO_BOMBA]  
   
//====================================================================================  
//                   TRANCO_BOMBA  
//====================================================================================  
  
namespace TRANCO_BOMBA  
{  
enum class ESTAGIOS : uint8_t  
 {  
       REPOUSO,  
       INICIANDO_MONITORAMENTO_HELICE,  
       BOMBA_TRAVADA,  
       PRIMEIRA_TENTATIVA_TRANCO,  
       SEGUNDA_TENTATIVA_TRANCO,   
       SENSOR_IR_DESATIVADO,  
       BOMBA_LIBERADA,
 };  
  
    void definir(ESTAGIOS novo);  
    ESTAGIOS atual();  
    const __FlashStringHelper* texto(ESTAGIOS s);  
      
}  
  
// [FIM::BAT::TRANCO_BOMBA]  
   
  
// [INSERE::BAT::ESTADOS_H_Eco_Hidro]  
