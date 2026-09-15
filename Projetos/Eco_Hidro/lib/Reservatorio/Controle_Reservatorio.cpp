//====================================================================================  
//                   DEPENDÊNCIAS  
//====================================================================================  
 
#include "Controle_Reservatorio.h" 
#include "DEBUG_MODE.h"

#include "Tempo.h"
#include "Cronometro.h"
 
#include "Estados_Sistema_Eco_Hidro.h"
#include "Driver_Boias.h"
#include "Leitura_Boias.h"
 
//====================================================================================  
//                   VARIÁVEIS INTERNAS  
//====================================================================================  

static constexpr uint32_t TEMPO_ENCHIMENTO_EXTRA_RESERVATORIO = TEMPO::HORAS(1); // 1 hora, mas pode ser ajustado depois dependendo do tempo que demora pra encher o reservatório (ou seja, o tempo que demora pra passar um pouco do excedente da máxima do reservatório 1 pra

 static CRONOMETRO::Temporizador enchimentoExcedente;

 static bool reservatorio2Maxima = false;
 static bool reservatorio1Minima = false;
 
//====================================================================================
//                   ATUALIZA BOIAS
//====================================================================================

static void Controle_Reservatorio_Atualiza_Boias() 
{ 

   reservatorio2Maxima = Leitura_Boias_Esta_Ativa(REGISTRO_BOIAS::RESERVATORIO_2_MAXIMA);
   reservatorio1Minima = !Leitura_Boias_Esta_Ativa(REGISTRO_BOIAS::RESERVATORIO_1_MINIMA);

}

//====================================================================================  
//                   SETUP  
//====================================================================================  
 
 void Controle_Reservatorio_Inicializa() 
{ 
 

 
} 
 
//====================================================================================  
//                   LIBERADO 
//====================================================================================  
 
void Controle_Reservatorio_Liberado() 
{ 

  Controle_Reservatorio_Atualiza_Boias();

            if (reservatorio1Minima || reservatorio2Maxima) {
                    
                enchimentoExcedente.Cronometro_Iniciar();


                RESERVATORIO::definir(RESERVATORIO::TRANSFERENCIA::SO_MAIS_UM_POUQUINHO);
            }
            
}

//====================================================================================
//                   SO_MAIS_UM_POUQUINHO
//====================================================================================

void Controle_Reservatorio_So_Mais_Um_Pouquinho()
{

            if (enchimentoExcedente.Cronometro_Dispara_Uma_Vez(TEMPO_ENCHIMENTO_EXTRA_RESERVATORIO))
             {

                RESERVATORIO::definir(RESERVATORIO::TRANSFERENCIA::FINALIZADO);
           
            }

}