//====================================================================================  
//                   DEPENDÊNCIAS  
//====================================================================================  
 
#include "Mediador_Sistema.h" 
#include "DEBUG_MODE.h" 

#include "Estados_Sistema_Eco_Hidro.h"
 
//====================================================================================  
//                   VARIÁVEIS INTERNAS  
//====================================================================================  


//====================================================================================
//                   RESETANDO LEITURA PROTECAO BOMBA
//====================================================================================

static void Mediador_Coordena_Primeiro_Ciclo_Casa_Torre()
{
    const auto casaTorre = CASA_TORRE::atual();
    const auto bombaCasa = BOMBA_CASA::atual();

    if (casaTorre == CASA_TORRE::ETAPAS::PRIMEIRO_CICLO_ENCHIMENTO_CAIXA
        && bombaCasa == BOMBA_CASA::ESTAGIOS::REPOUSO)
    {
        LUZ_ACESSO::definir(LUZ_ACESSO::SITUACAO::DESLIGANDO); //força desligamento "de forma burra, evitando condições desnecessárias"
        LUZ_ESTACIONAMENTO::definir(LUZ_ESTACIONAMENTO::SITUACAO::DESLIGANDO); //idem

        POTENCIA::definir(POTENCIA::SELECIONADO::BOMBA_CASA);
        DESLIGAMENTO_BOMBA::definir(DESLIGAMENTO_BOMBA::MOTIVO::NENHUM); //resetando o registro anterior informado.
        UNIDADE_POTENCIA::definir(UNIDADE_POTENCIA::ESTAGIOS::ACIONAMENTO_RELES);
    }
}

//====================================================================================
//                   CONFIRMA INVERSOR E RELE 12V ATIVO
//====================================================================================

static void Mediador_Coordena_Unidade_Potencia_Ligado()
{

    auto const unidadePotencia = UNIDADE_POTENCIA::atual();
    auto const potenciaSelecionado = POTENCIA::atual();
    auto const bombaCasa = BOMBA_CASA::atual();
    auto const bomba12V = BOMBA_12V::atual();
//  auto const portao = PORTAO_ELETRONICO::atual();

    if (unidadePotencia == UNIDADE_POTENCIA::ESTAGIOS::LIGADO)
    {

        if (bombaCasa == BOMBA_CASA::ESTAGIOS::AGUARDA_LIBERACAO
        && potenciaSelecionado == POTENCIA::SELECIONADO::BOMBA_CASA)
        {   
            BOMBA_CASA::definir(BOMBA_CASA::ESTAGIOS::LIGANDO_BOMBA);
        }

        if (bomba12V == BOMBA_12V::ESTAGIOS::AGUARDA_LIBERACAO
        && potenciaSelecionado == POTENCIA::SELECIONADO::BOMBA_12V)
        {
            BOMBA_12V::definir(BOMBA_12V::ESTAGIOS::LIGANDO_BOMBA);
        }

        /*  
        if (portao == PORTAO_ELETRONICO::SITUACAO::AGUARDA_LIBERACAO
        && potenciaSelecionado == POTENCIA::SELECIONADO::PORTAO)
        {
            PORTAO_ELETRONICO::definir(PORTAO_ELETRONICO::SITUACAO::LIGANDO_BOMBA);
        }
        */
    }

}

//@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
//
//                                                VOLTAGEM DE CONTROLE DA CARGA
//
//@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
//==========================================================================================================
//                   VOLTAGEM DE CONTROLE DA CARGA (CONSUMINDO ENERGIA) - ACIONAMENTO DAS LUZES PARA CONSUMO
//==========================================================================================================

static void Mediador_Coordena_Voltagem_Manter_Carga() //14.4V
{

      const auto bateria = VOLTAGEM::atual();

    if (bateria == VOLTAGEM::LEITURA::MANTENDO_CARGA) {

        LUZ_ESTACIONAMENTO::definir(LUZ_ESTACIONAMENTO::SITUACAO::CONSUMINDO);
        LUZ_ACESSO::definir(LUZ_ACESSO::SITUACAO::CONSUMINDO);

    }

}

//==============================================================================================
//                   VOLTAGEM DE CONTROLE DA CARGA (CONSUMO REALIZADO) - DESLIGAMENTO DAS LUZES 
//==============================================================================================

static void Mediador_Coordena_Voltagem_Consumo_Realizado() //14.2V
{

      const auto luzEstacionamento = LUZ_ESTACIONAMENTO::atual();
      const auto luzAcesso = LUZ_ACESSO::atual();
      const auto bateria = VOLTAGEM::atual();

    if 
        (bateria == VOLTAGEM::LEITURA::CARGA_CONSUMIDA
        && (luzEstacionamento == LUZ_ESTACIONAMENTO::SITUACAO::CONSUMINDO
        || luzAcesso == LUZ_ACESSO::SITUACAO::CONSUMINDO)) {

        LUZ_ESTACIONAMENTO::definir(LUZ_ESTACIONAMENTO::SITUACAO::DESLIGANDO);
        LUZ_ACESSO::definir(LUZ_ACESSO::SITUACAO::DESLIGANDO);

    }

}

//@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
//
//                                                PORTÃO ELETRÔNICO
//
//@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
//====================================================================================
//                   MOMENTO PORTÃO ELETRONICO
//====================================================================================

static void Mediador_Coordena_Portao() 
{

        const auto portao = PORTAO_ELETRONICO::atual();

if (portao == PORTAO_ELETRONICO::SITUACAO::LIBERADO)

    {
  //      BOMBA_12V::definir(BOMBA_12V::SITUACAO::DESLIGAMENTO_FORCADO);
   //     BOMBA_CASA::definir(BOMBA_CASA::ESTAGIOS::DESLIGAMENTO_FORCADO);
        SISTEMA_ECO_HIDRO::definir(SISTEMA_ECO_HIDRO::MODO::BLOQUEIO);
    }
}
//mais fuções do portão entraram aqui assim como o LoRa.



//====================================================================================  
//                   SETUP  
//====================================================================================  
 
 void MEDIADOR_BOOT() 
{ 
    
    CASA_TORRE::definir(CASA_TORRE::ETAPAS::NORMAL);
    BOMBA_CASA::definir(BOMBA_CASA::ESTAGIOS::REPOUSO);  
    RESERVATORIO::definir(RESERVATORIO::TRANSFERENCIA::NORMAL);                        
    BOMBA_12V::definir(BOMBA_12V::ESTAGIOS::REPOUSO);  
    SISTEMA_ECO_HIDRO::definir(SISTEMA_ECO_HIDRO::MODO::NORMAL);           
    LUZ_ESTACIONAMENTO::definir(LUZ_ESTACIONAMENTO::SITUACAO::DESLIGADO);                
    LUZ_ACESSO::definir(LUZ_ACESSO::SITUACAO::DESLIGADO);               
    VOLTAGEM::definir(VOLTAGEM::LEITURA::NORMAL);             
    DESLIGAMENTO_BOMBA::definir(DESLIGAMENTO_BOMBA::MOTIVO::NENHUM);    
    PORTAO_ELETRONICO::definir(PORTAO_ELETRONICO::SITUACAO::REPOUSO);           
    UNIDADE_POTENCIA::definir(UNIDADE_POTENCIA::ESTAGIOS::REPOUSO);
    POTENCIA::definir(POTENCIA::SELECIONADO::REPOUSO);

} 
 
//====================================================================================  
//                   LOOP  
//====================================================================================  
 
 void MEDIADOR_LOOP() 
{

    Mediador_Coordena_Primeiro_Ciclo_Casa_Torre();

    Mediador_Coordena_Unidade_Potencia_Ligado();

    Mediador_Coordena_Voltagem_Manter_Carga();
    
    Mediador_Coordena_Voltagem_Consumo_Realizado(); 

    Mediador_Coordena_Portao();

}