//====================================================================================  
//                   DEPENDÊNCIAS  
//====================================================================================  
 
#include "Leitura_Botao.h" 
#include "DEBUG_MODE.h" 
 
#include "Modulo_Botao.h"
 
//====================================================================================  
//                   VARIÁVEIS INTERNAS  
//====================================================================================  
 
static bool leituraBotaoAtual    = false;
static bool leituraAnteriorBotao = false;
static bool cliqueBotaoDetectado = false; 

//====================================================================================  
//                   SETUP  
//====================================================================================  
 
 void Leitura_Botao_Inicializa() 
{ 
        debugSetupDivisao(COR::AMARELO, "LEITURA BOTÃO INICIALIZANDO", COR::MAGENTA, "=", 45);
} 
 
//====================================================================================
//                   LOOP
//====================================================================================

void Leitura_Botao_Atualiza()
{
    leituraAnteriorBotao = leituraBotaoAtual;

    leituraBotaoAtual = Modulo_Botao_Esta_Pressionado(); //@@ Modulo_Botao.cpp

    // Detecta borda de subida (apertou)
    if (leituraBotaoAtual && !leituraAnteriorBotao)
    {
        cliqueBotaoDetectado = true;
    }
}

//====================================================================================
//                   EVENTO
//====================================================================================


bool Leitura_Botao_Foi_Pressionado()
{
    if (cliqueBotaoDetectado)
    {
        cliqueBotaoDetectado = false;
        return true;
    }

    return false;
}