//====================================================================================  
//                   DEPENDÊNCIAS  
//====================================================================================  
 
#include "Sistema_Reservatorio.h" 
#include "DEBUG_MODE.h" 

#include "Gerencia_Bomba_12V.h"
#include "Gerencia_Reservatorio.h"
#include "Controle_Reservatorio.h"
#include "Controle_Bomba_12V.h"

#include "Estados_Sistema_Eco_Hidro.h"

 
//====================================================================================  
//                   VARIÁVEIS INTERNAS  
//====================================================================================  
 
 static void Coordena_Transferencia_Reservatorios();
 static void Coordena_Finalizacao_Transferencia_Reservatorios();
 
//====================================================================================  
//                   SETUP  
//====================================================================================  
 
 void RESERVATORIO_BOOT() 
{ 

//    Gerencia_Reservatorio_Inicializa();

//    Controle_Reservatorio_Inicializa();

//    Gerencia_Bomba_12V_Inicializa();

//    Controle_Bomba_12V_Inicializa();
 
} 
 
//====================================================================================  
//                   LOOP  
//====================================================================================  
 
 void RESERVATORIO_LOOP() 
{ 
 
    Gerencia_Reservatorio_Atualiza();

    Gerencia_Bomba_12V_Atualiza();

    Coordena_Transferencia_Reservatorios();

    Coordena_Finalizacao_Transferencia_Reservatorios() ;


} 


//====================================================================================
//                   MOMENTO BOMBA 12V RESERVATORIO (ACIONADO)
//====================================================================================

static void Coordena_Transferencia_Reservatorios() 
{

            const auto casaTorre = CASA_TORRE::atual();
            const auto bateria = VOLTAGEM::atual();
            const auto reservatorio = RESERVATORIO::atual();
            const auto bomba12V = BOMBA_12V::atual();
            const auto sistemaModo = SISTEMA_ECO_HIDRO::atual();

    if (casaTorre == CASA_TORRE::ETAPAS::FINALIZADO
    && reservatorio == RESERVATORIO::TRANSFERENCIA::NORMAL
    && bomba12V == BOMBA_12V::ESTAGIOS::REPOUSO //trava segurança evita acesso repentino nesse if.
    && bateria == VOLTAGEM::LEITURA::CHEIA
    && sistemaModo != SISTEMA_ECO_HIDRO::MODO::BLOQUEIO)   //se for bloqueio do sistema logo nunca acessará essa parte do if.)
    
    {
        RESERVATORIO::definir(RESERVATORIO::TRANSFERENCIA::LIBERADO);
        BOMBA_12V::definir(BOMBA_12V::ESTAGIOS::LIBERADO);
    }

}

//====================================================================================
//                   DESLIGAMENTO DA BOMBA DO RESERVATÓRIO
//====================================================================================

static void Coordena_Finalizacao_Transferencia_Reservatorios() 
{

            const auto reservatorio = RESERVATORIO::atual();

    if (reservatorio == RESERVATORIO::TRANSFERENCIA::FINALIZADO)
    
    {
        BOMBA_12V::definir(BOMBA_12V::ESTAGIOS::DESLIGANDO_BOMBA);
        RESERVATORIO::definir(RESERVATORIO::TRANSFERENCIA::NORMAL);
        CASA_TORRE::definir(CASA_TORRE::ETAPAS::NORMAL);
    }
}