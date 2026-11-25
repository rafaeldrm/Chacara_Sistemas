//-------------------------- DEPENDÊNCIAS ----------------------------------//

#pragma once
#include <Arduino.h>

//-------------------------- ESTRUTURAS DE ESTADOS DO SISTEMA GATE DRIMEL ----------------------------------// 
     
//====================================================================================    
//                   POSICIONAMENTO PORTAO   (botoeira)
//====================================================================================    
    
namespace POSICIONAMENTO_PORTAO   
{    
enum class ESTADO : uint8_t    
 {    
       DESCONHECIDO,
       ABRINDO,
       ABERTO,
       FECHANDO,
       FECHADO,

 };    
    
    void definir(ESTADO novo);
    void atualizarCiclo();     
    ESTADO atual();
    const __FlashStringHelper* texto(ESTADO s);    
        
}

//====================================================================================    
//                   TRINCO PORTAO 
//====================================================================================    
    
namespace TRINCO_PORTAO   
{    
enum class ESTADO : uint8_t    
 {    
       DESCONHECIDO,
       SINALIZADO_COMO_ABERTO,
       SINALIZADO_COMO_FECHADO,
       FALHA_SENSOR,
       
 };    
    
    void definir(ESTADO novo);
    void atualizarCiclo();     
    ESTADO atual();
    const __FlashStringHelper* texto(ESTADO s);    
        
}

//====================================================================================    
//                   TRANCA MAGNÉTICA
//====================================================================================    
    
namespace TRANCA_MAGNETICA  
{    
enum class ESTADO : uint8_t    
 {    
       TRANCADO,
       SOLTO,
       LENDO_VOLTAGEM,
       
 };    
    
    void definir(ESTADO novo);
    void atualizarCiclo();     
    ESTADO atual();
    const __FlashStringHelper* texto(ESTADO s);    
        
}


//====================================================================================    
//                   FORA CHÁCARA (sensor IR)
//====================================================================================    
    
namespace FORA_CHACARA  
{    
enum class ESTADO : uint8_t    
 {    
       REPOUSO,
       LENDO,
       VEICULO_PRESENTE,
       VEICULO_AUSENTE,
       
 };    
    
    void definir(ESTADO novo);
    void atualizarCiclo();     
    ESTADO atual();
    const __FlashStringHelper* texto(ESTADO s);    
        
}

//====================================================================================    
//                   DENTRO CHÁCARA (sensor IR)
//====================================================================================    
    
namespace DENTRO_CHACARA  
{    
enum class ESTADO : uint8_t    
 {    
       REPOUSO,
       LENDO,
       VEICULO_PRESENTE,
       VEICULO_AUSENTE,
       
 };    
    
    void definir(ESTADO novo);
    void atualizarCiclo();     
    ESTADO atual();
    const __FlashStringHelper* texto(ESTADO s);    
        
}