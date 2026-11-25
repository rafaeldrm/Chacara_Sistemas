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
enum class ESTADO : uint8_t    
 {    
       NORMAL,   
       PRIMEIRO_CICLO_ENCHIMENTO_CAIXA,   
       AGUARDANDO_TRANSFERENCIA_GRAVIDADE,   
       SEGUNDO_CICLO_ENCHIMENTO_CAIXA,   
       FINALIZADO,
       LIBERADO_PARA_O_RESERVATORIO,
       BLOQUEIO,   
 };    
    
    void definir(ESTADO novo);
    void atualizarCiclo();     
    ESTADO atual();
    const __FlashStringHelper* texto(ESTADO s);    
        
}    
    
// [FIM::BAT::CASA_TORRE]    
   
// [INICIO::BAT::BOMBA_CASA]      
       
//====================================================================================      
//                   BOMBA_CASA      
//====================================================================================      
      
namespace BOMBA_CASA      
{      
enum class ESTADO : uint8_t      
 {      
      REPOUSO,        
      LIBERADO,        
      AGUARDA_LIBERACAO,       
      LIGANDO_BOMBA,        
      BOMBA_LIGADA,        
      DESLIGANDO_BOMBA,
      BLOQUEIO,
      BLOQUEADO,      
 };      
      
    void definir(ESTADO novo);
    void atualizarCiclo();    
    ESTADO atual();
    const __FlashStringHelper* texto(ESTADO s);      
          
}      
      
// [FIM::BAT::BOMBA_CASA]      
                  
// [INICIO::BAT::RESERVATORIO]                           
                            
//====================================================================================                           
//                   RESERVATORIO                           
//====================================================================================                           
                           
namespace RESERVATORIO                           
{                           
enum class ESTADO : uint8_t                           
 {                           
       NORMAL,                           
       LIBERADO,
       TRANSFERENCIA_RESERVATORIOS,                          
       SO_MAIS_UM_POUQUINHO,    
       FINALIZADO,                          
       BLOQUEIO_SEGURANCA,                           
 };                           
                           
    void definir(ESTADO novo);
    void atualizarCiclo();            
    ESTADO atual();                         
    const __FlashStringHelper* texto(ESTADO s);                           
                               
}                           
                           
// [FIM::BAT::RESERVATORIO]                           
                            
// [INICIO::BAT::BOMBA_12V]         
          
//====================================================================================         
//                   BOMBA_12V         
//====================================================================================         
         
namespace BOMBA_12V         
{         
enum class ESTADO : uint8_t         
 {         
      REPOUSO,        
      LIBERADO,        
      AGUARDA_LIBERACAO,       
      LIGANDO_BOMBA,        
      BOMBA_LIGADA,        
      DESLIGANDO_BOMBA,             
 };         
         
    void definir(ESTADO novo);
    void atualizarCiclo();  
    ESTADO atual();         
    const __FlashStringHelper* texto(ESTADO s);         
             
}         
         
// [FIM::BAT::BOMBA_12V]         
                 
// [INICIO::BAT::SISTEMA]                 
                  
//====================================================================================                 
//                   SISTEMA                 
//====================================================================================                 
                 
namespace SISTEMA_ECO_HIDRO                 
{                 
enum class ESTADO : uint8_t                 
 {                 
       NORMAL,                
       CICLO_COMPLETO,                
       MEIO_CICLO,                
       BLOQUEIO,                
       CONTADOR_PULSO,                
 };                 
                 
    void definir(ESTADO novo);
    void atualizarCiclo();                
    ESTADO atual();                 
    const __FlashStringHelper* texto(ESTADO s);                 
                     
}                 
                 
// [FIM::BAT::SISTEMA]                              
                         
// [INICIO::BAT::LUZ_ESTACIONAMENTO]                        
                         
//====================================================================================                        
//                   LUZ_ESTACIONAMENTO                        
//====================================================================================                        
                        
namespace LUZ_ESTACIONAMENTO                        
{                        
enum class ESTADO : uint8_t                        
 {                        
       DESLIGADO,   
       DESLIGANDO,
       INICIANDO_TEMPORIZACAO,                     
       TEMPORIZADO,                        
       FORCADO,   
       CONSUMINDO,
       ACIONADO,  
   
 };                        
                        
    void definir(ESTADO novo);
    void atualizarCiclo();                         
    ESTADO atual();                       
    const __FlashStringHelper* texto(ESTADO s);                        
                            
}                        
                        
// [FIM::BAT::LUZ_ESTACIONAMENTO]                        
              
// [INICIO::BAT::LUZ_ACESSO]                    
                     
//====================================================================================                    
//                   LUZ_ACESSO                    
//====================================================================================                    
                    
namespace LUZ_ACESSO                    
{                    
enum class ESTADO : uint8_t                    
 {                    
       DESLIGADO,   
       DESLIGANDO,
       INICIANDO_TEMPORIZACAO,                  
       TEMPORIZADO,   
       FORCADO,   
       CONSUMINDO,
       ACIONADO,   
 };                    
                    
    void definir(ESTADO novo);
    void atualizarCiclo();                     
    ESTADO atual();                     
    const __FlashStringHelper* texto(ESTADO s);                    
                        
}                    
                    
// [FIM::BAT::LUZ_ACESSO]                    
                  
// [INICIO::BAT::VOLTAGEM]                      
                       
//====================================================================================                      
//                   VOLTAGEM                      
//====================================================================================                      
                      
namespace VOLTAGEM                      
{                      
enum class ESTADO : uint8_t                      
 {                      
       NORMAL,                      
       IDEAL,                      
       CHEIA,   
       MANTENDO_CARGA,   
       CARGA_CONSUMIDA,                   
       CRITICO,
       CARGA_PRIORIDADE,                      
 };                      
                      
    void definir(ESTADO novo);
    void atualizarCiclo();                   
    ESTADO atual();                     
    const __FlashStringHelper* texto(ESTADO s);                      
                          
}                      
                      
// [FIM::BAT::VOLTAGEM]                      
        
// [INICIO::BAT::DESLIGAMENTO_BOMBA]          
           
//====================================================================================          
//                   DESLIGAMENTO_BOMBA          
//====================================================================================          
          
namespace DESLIGAMENTO_BOMBA          
{          
enum class ESTADO : uint8_t          
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
          
    void definir(ESTADO novo);
    void atualizarCiclo();           
    ESTADO atual();          
    const __FlashStringHelper* texto(ESTADO s);          
              
}          
          
// [FIM::BAT::DESLIGAMENTO_BOMBA]          
                 
// [INICIO::BAT::PORTAO_ELETRONICO]                    
                     
//====================================================================================                    
//                   PORTAO_ELETRONICO                    
//====================================================================================                    
                    
namespace PORTAO                    
{                    
enum class ESTADO : uint8_t                    
 {                    
      REPOUSO,        
      LIBERADO,        
      AGUARDA_LIBERACAO,       
      LIGANDO_PORTAO,        
      PORTAO_LIGADO,        
      DESLIGANDO_PORTAO,
 };                    
                    
    void definir(ESTADO novo);
    void atualizarCiclo();                     
    ESTADO atual();                  
    const __FlashStringHelper* texto(ESTADO s);                    
                        
}                    
                    
// [FIM::BAT::PORTAO_ELETRONICO]                    
   
// [INICIO::BAT::UNIDADE_POTENCIA]   
   
//====================================================================================   
//                   UNIDADE_POTENCIA (Controle Central do Inversor)   
//====================================================================================   
   
namespace UNIDADE_POTENCIA   
{   
enum class ESTADO : uint8_t   
 {   
       REPOUSO,   
       ACIONAMENTO_RELES,   
       AGUARDANDO_ESTABILIZAR,   
       LIGADO,   
       DESLIGANDO,   
 };   
   
    void definir(ESTADO novo);
    void atualizarCiclo(); 
    ESTADO atual();    
    const __FlashStringHelper* texto(ESTADO s);   
   
}   
   
// [FIM::BAT::UNIDADE_POTENCIA]   
   
// [INICIO::BAT::POTENCIA]   
   
//====================================================================================   
//                   POTENCIA (Qual está atualmente em uso)   
//====================================================================================   
   
namespace POTENCIA   
{   
enum class ESTADO : uint8_t   
      {   
            REPOUSO,   
            BOMBA_CASA,   
            BOMBA_12V,   
            PORTAO,   
            DESLIGANDO,   
      };   
         
      void definir(ESTADO novo);
      void atualizarCiclo(); 
      ESTADO atual();   
      const __FlashStringHelper* texto(ESTADO s);   
         
      }   
   
// [FIM::BAT::POTENCIA]      
   
// [INICIO::BAT::TRANCO_BOMBA_CASA]  
   
//====================================================================================  
//                   TRANCO_BOMBA_CASA  
//====================================================================================  
  
namespace TRANCO_BOMBA_CASA  
{  
enum class ESTADO : uint8_t  
 {  
       REPOUSO,  
       INICIANDO_MONITORAMENTO_HELICE,  
       BOMBA_TRAVADA,  
       PRIMEIRA_TENTATIVA_TRANCO,  
       SEGUNDA_TENTATIVA_TRANCO,   
       SENSOR_IR_DESATIVADO,  
       BOMBA_LIBERADA,
 };  
  
    void definir(ESTADO novo);
    void atualizarCiclo();   
    ESTADO atual();  
    const __FlashStringHelper* texto(ESTADO s); 
      
}  
  
// [FIM::BAT::TRANCO_BOMBA_CASA] 

// [INICIO::BAT::PROTECAO_TERMICA_BOMBA_CASA]

//====================================================================================
//                   PROTECAO_TERMICA_BOMBA_CASA
//====================================================================================

namespace PROTECAO_TERMICA_BOMBA_CASA
{
enum class ESTADO : uint8_t
 {
      REPOUSO,
      MONITORAMENTO_ATIVADO,
      SUPERAQUECIMENTO,
      MODO_SEGURANCA_PRIMEIRO_CICLO,
      MODO_SEGURANCA_SEGUNDO_CICLO,
      FINALIZADO,
 };
      void definir(ESTADO novo);
      void atualizarCiclo();
      ESTADO atual();
      const __FlashStringHelper* texto(ESTADO s);
}

// [FIM::BAT::PROTECAO_TERMICA_BOMBA_CASA]

// [INICIO::BAT::GIRO_PROTECAO_BOMBA_CASA]

//====================================================================================
//                   GIRO_PROTECAO_BOMBA_CASA
//====================================================================================

namespace GIRO_PREVENTIVO_BOMBA_CASA
{
enum class ESTADO : uint8_t
      {     
            REPOUSO,
            INICIA_CONTAGEM_COTIDIANO,
            CANCELA_CONTAGEM_COTIDIANO,
            VERIFICA_CONTAGEM_COTIDIANO,
            PREPARA_EXERCICIO_BOMBA,
            INICIO_EXERCICIO_BOMBA,
            FIM_EXERCICIO_BOMBA,
            FINALIZADO,      
      };

      void definir(ESTADO novo);
      void atualizarCiclo();
      ESTADO atual();
      const __FlashStringHelper* texto(ESTADO s);
}


// [FIM::BAT::GIRO_PROTECAO_BOMBA_CASA]

// [INSERE::BAT::ESTADOS_H_Eco_Hidro]  