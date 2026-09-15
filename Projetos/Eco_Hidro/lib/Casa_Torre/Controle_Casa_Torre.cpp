//====================================================================================  
//                   DEPENDÊNCIAS  
//====================================================================================  
 
#include "Controle_Casa_Torre.h" 
#include "DEBUG_MODE.h" 
 
#include "Tempo.h"
#include "Cronometro.h"

#include "Estados_Sistema_Eco_Hidro.h"
#include "Boias_ID.h"
#include "Leitura_Boias.h"
 
//====================================================================================  
//                   VARIÁVEIS INTERNAS  
//====================================================================================  
 
// tempo de descida da água Torre -> Casa
static constexpr uint32_t TEMPO_GRAVIDADE = TEMPO::HORAS(2) + TEMPO::MINUTOS(45); //(2UL * 60UL * 60UL * 1000UL) + (45UL * 60UL * 1000UL); // 2h45min //@@ Tempo.h

static CRONOMETRO::Temporizador transferenciaTorreCasa; //@@ Cronometro.h

static bool casaMinimaVazia = false;
static bool torreMaximaCheia = false;

//====================================================================================
//                   FUNÇÃO ATUALIZA BOIAS
//====================================================================================

static void Controle_Casa_Torre_Atualiza_Boias()
{

    casaMinimaVazia = !Leitura_Boias_Esta_Ativa(REGISTRO_BOIAS::CASA_MINIMA);   //@@ Leitura_Boias.cpp
    torreMaximaCheia = Leitura_Boias_Esta_Ativa(REGISTRO_BOIAS::TORRE_MAXIMA);  //@@ Leitura_Boias.cpp

}
 
//====================================================================================  
//                   SETUP  
//====================================================================================  
 
 void Controle_Casa_Torre_Inicializa() 
{ 
 
 
}
 
//====================================================================================  
//                   NORMAL 
//====================================================================================  
 
 void Controle_Casa_Torre_Normal()
{ 
 
     Controle_Casa_Torre_Atualiza_Boias();

      if (casaMinimaVazia) 
        {
            CASA_TORRE::definir(CASA_TORRE::ETAPAS::PRIMEIRO_CICLO_ENCHIMENTO_CAIXA);
        }

} 

//====================================================================================
//                   PRIMEIRO CICLO ENCHIMENTO CAIXA
//====================================================================================

void Controle_Casa_Torre_Primeiro_Ciclo_Enchimento_Caixa()
{

      Controle_Casa_Torre_Atualiza_Boias();

     if (torreMaximaCheia)
        {
            CASA_TORRE::definir(CASA_TORRE::ETAPAS::AGUARDANDO_TRANSFERENCIA_GRAVIDADE);
                
            transferenciaTorreCasa.Cronometro_Iniciar();
        }
}

//====================================================================================
//                   AGUARDANDO TRANSFERÊNCIA CAIXA TORRE -> CASA
//====================================================================================

void Controle_Casa_Torre_Aguardando_Transferencia_Gravidade()
{

     if (transferenciaTorreCasa.Cronometro_Dispara_Uma_Vez(TEMPO_GRAVIDADE))
        {
            CASA_TORRE::definir(CASA_TORRE::ETAPAS::SEGUNDO_CICLO_ENCHIMENTO_CAIXA);
        }
}

//====================================================================================
//                   SEGUNDO CICLO ENCHIMENTO CAIXA
//====================================================================================

void Controle_Casa_Torre_Segundo_Ciclo_Enchimento_Caixa()
{

    Controle_Casa_Torre_Atualiza_Boias();

        if (torreMaximaCheia)
            {
                CASA_TORRE::definir(CASA_TORRE::ETAPAS::FINALIZADO);
            }        
}