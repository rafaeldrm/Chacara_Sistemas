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

//[INICIO::BAT::ORDENS_DRIMEL_SYSTEM::CASA_TORRE]

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

//[FIM::BAT::ORDENS_DRIMEL_SYSTEM::CASA_TORRE]

//[INICIO::BAT::ORDENS_DRIMEL_SYSTEM::LUZ_ACESSO]

      namespace LUZ_ACESSO   
      {   
            enum class ORDEM : uint8_t   
      {   
            NENHUM,   
            TEMPORIZACAO,   
            FORCADO,   
            DESLIGA,    
      };   
         
      void definir(ORDEM novo);   
      ORDEM atual();   
      const __FlashStringHelper* texto(ORDEM s);   
         
      } 

//[FIM::BAT::ORDENS_DRIMEL_SYSTEM::LUZ_ACESSO]

//[INICIO::BAT::ORDENS_DRIMEL_SYSTEM::LUZ_ESTACIONAMENTO]

      namespace LUZ_ESTACIONAMENTO  
      {   
            enum class ORDEM : uint8_t   
      {   
            NENHUM,   
            TEMPORIZACAO,   
            FORCADO,   
            DESLIGA,    
      };   
         
      void definir(ORDEM novo);   
      ORDEM atual();   
      const __FlashStringHelper* texto(ORDEM s);   
         
      }  
      
//[FIM::BAT::ORDENS_DRIMEL_SYSTEM::LUZ_ESTACIONAMENTO]

//[PROXIMO::BAT::ORDENS_DRIMEL_SYSTEM]

}   
   
// [FIM::BAT::ORDENS_DRIMEL_SYSTEM]

// [INSERE::BAT::ESTADOS_EVENTOS_H_Eco_Hidro]  