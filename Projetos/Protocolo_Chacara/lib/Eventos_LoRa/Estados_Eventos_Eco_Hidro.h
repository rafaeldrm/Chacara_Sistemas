//-------------------------- DEPENDÊNCIAS ----------------------------------//

#pragma once
#include <Arduino.h>

//-------------------------- ESTRUTURAS DE ESTADOS DO SISTEMA Eco_Hidro ----------------------------------//     

// [INICIO::BAT::ORDENS_DRIMEL_SYSTEM]   
   
//====================================================================================   
//                   ORDENS DRIMEL SYSTEM   
//====================================================================================   
   
namespace ORDENS_DRIMEL_SYSTEM   
{   

      //====================================================================================
      //                   CASA TORRE
      //====================================================================================

      namespace CASA_TORRE   
      {   
            enum class ORDEM : uint8_t   
      {   
            NENHUM,   
            CICLO_COMPLETO,   
            MEIO_CICLO,   
            BLOQUEIA,   
            DESLIGA,   
      };   
         
      void definir(ORDEM novo);   
      ORDEM atual();   
      const __FlashStringHelper* texto(ORDEM s);   
         
      }

      //====================================================================================
      //                   RESERVATÓRIO
      //====================================================================================
      
      namespace RESERVATORIO
      {
            enum class ORDEM : uint8_t
            {
                  NENHUM,
                  TRANSFERENCIA_ENTRE_RESERVATORIOS,
                  BLOQUEIA,
                  DESLIGA,
            };

      void definir(ORDEM novo);
      ORDEM atual();
      const __FlashStringHelper* texto(ORDEM s);

      }

      //====================================================================================
      //                   LUZ ACESSO RESERVATÓRIO
      //====================================================================================
      
      namespace LUZ_ACESSO   
      {   
            enum class ORDEM : uint8_t   
      {   
            NENHUM,   
            TEMPORIZACAO,
            ACIONADO,   
            FORCADO,   
            DESLIGA,    
      };   
         
      void definir(ORDEM novo);   
      ORDEM atual();   
      const __FlashStringHelper* texto(ORDEM s);   
         
      } 

      //====================================================================================
      //                   LUZ ESTACIONAMENTO
      //====================================================================================
      
      namespace LUZ_ESTACIONAMENTO  
      {   
            enum class ORDEM : uint8_t   
      {   
            NENHUM,   
            TEMPORIZACAO,
            ACIONADO,   
            FORCADO,   
            DESLIGA,    
      };   
         
      void definir(ORDEM novo);   
      ORDEM atual();   
      const __FlashStringHelper* texto(ORDEM s);   
         
      }  
}   