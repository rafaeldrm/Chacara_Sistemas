//====================================================================================  
//                   DEPENDÊNCIAS  
//====================================================================================  
 
#include "Tempo_Boias.h" 
#include "DEBUG_MODE.h" 
 
#include "Tempo.h" 
#include "Cronometro.h"
 
//====================================================================================  
//                   VARIÁVEIS INTERNAS  
//====================================================================================  

static constexpr uint32_t TEMPO_CONFIRMACAO = TEMPO::SEGUNDOS(20);

static CRONOMETRO::Temporizador boiasLeitura;
 
//====================================================================================  
//                   SETUP  
//====================================================================================  
 
void Tempo_Boias_Inicializa() 
{ 
    debugSetupDivisao(COR::AMARELO, "TEMPO BOIAS INICIALIZANDO", COR::MAGENTA, "=", 45);
      
    boiasLeitura.Cronometro_Iniciar();
} 

//====================================================================================  
//                   PROCESSA TEMPO DE VERIFICAÇÃO DAS BÓIAS
//====================================================================================  
 
bool Tempo_Boias_Processa()
{
    return boiasLeitura.Cronometro_Ciclo_Continuo(TEMPO_CONFIRMACAO);
}