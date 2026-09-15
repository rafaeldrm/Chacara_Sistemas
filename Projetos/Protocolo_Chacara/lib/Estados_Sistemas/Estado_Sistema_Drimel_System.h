#pragma once
#include <Arduino.h>

namespace SISTEMA_DRIMEL_SYSTEM 
{
    
    //====================================================================================
    //                   SISTEMA MODOS
    //====================================================================================
    
enum class ESTADO : uint8_t 
  { //aqui posso desmemmbrar os estados existentes para outras FSM, caso eu precise ter mais de um modo ao mesmo tempo
        NORMAL,  //Modo Normal de Funcionamento
        FERIAS,  //Modo Férias tornando muitas funções inativas e módulos desligados
        BLOQUEADO,  //Modo de segurança contra possíveis ataques externos
        MANUTENCAO,  //Modo de Manutenção, desativa logs, salvamentos de informações importantes no SD e EEPROM
        FESTA,       //Modo Festa, gera mais proteção contra possíveis pessoas inoportunas
        EMERGENCIA,   //modo de Emergencia, possíveis acidentes na chácara, entra em modo de alerta absoluto
        NOTURNO      //modo para que o sistema fique estudando e aprendendo com ele mesmo.
  };

    void definir(ESTADO novo);
    ESTADO atual();
    const __FlashStringHelper* texto(ESTADO s);

}

//====================================================================================
//                          IA
//====================================================================================

namespace IA
{

    //====================================================================================
    //                   PERSONALIDADE DA INTELIGÊNCIA ARTIFICIAL
    //===================================================================================
    namespace PERSONALIDADE 
    {

        enum class PERSONAGEM : uint8_t {
            KAMYLLA,  //atua como conselheira, amiga, até namoradinha animada para o Rafael Drimel
            SPYRO     //atua como ajudante, conselheiro, amigo para a Dulce (Mãe de Rafael)
    };

    void definir(PERSONAGEM novo);
    PERSONAGEM atual();
    const __FlashStringHelper* texto(PERSONAGEM p);
    
    }

    //====================================================================================
    //                   SENTIMENTOS DA INTELIGÊNCIA ARTIFICIAL
    //====================================================================================

    namespace SENTIMENTOS 
    {   
    enum class AVALIACAO : uint8_t {
            NENHUM, //resultado inicial e de repouso
            BOM,   //Bonificação por tomar decisão boa nos sistemas
            RUIM,  //penalização por tomar decisões erradas nos sistemas
            NEUTRO  //indiferente
    };

        void definir(AVALIACAO novo);
        AVALIACAO atual();
        const __FlashStringHelper* texto(AVALIACAO s);
    }

    //====================================================================================
    //                   INTELIGÊNCIA ARTIFICIAL, ESTÁGIOS
    //====================================================================================

    namespace ESTAGIOS 
    {
    enum class ESTADO : uint8_t {
            APRENDENDO,  //aqui a IA vai conhecer e aprender pegando informações dos sistemas e da chácara
            TESTANDO,    //modo teste onde ela simulará decisões
            ESTUDANDO,   //modo de estudo nas madrugadas com os logs
            DECISAO,     //modo de tomar decisões apenas perguntando se é valido ou não
            AUTOMATICO,   //modo funcional, aprendido e treinado, liderando a chácara como um todo
            SIMULANDO   //modo de simulação, onde ela vai simular decisões e ações para ver se é valido ou não
    };

    void definir(ESTADO novo);
        ESTADO atual();
        const __FlashStringHelper* texto(ESTADO e);
        }
}


//====================================================================================
//                   DISPLAY TFT 4.0
//====================================================================================

namespace  TELA {

    //====================================================================================
    //                   MENU
    //====================================================================================
    

    namespace MENU {

    enum class OPCAO : uint8_t {
        NORMAL,
        MANUTENCAO,

    };

    void definir(OPCAO novo);
    OPCAO atual();
    const __FlashStringHelper* texto(OPCAO m);

}

//====================================================================================
//                   MANUTENÇÃO
//====================================================================================

    namespace MANUTENCAO {

        enum class ETAPAS : uint8_t {
            NENHUM,
            ATUALIZAR,
            DESLIGAMENTO_SEGURO,
        };

    void definir(ETAPAS novo);
    ETAPAS atual();
    const __FlashStringHelper* texto(ETAPAS s);

    }

}




//====================================================================================
//                   ECO HIDRO
//====================================================================================


namespace ECO_HIDRO
{

//====================================================================================
//                   CAIXA ENTRADA
//====================================================================================

    namespace CAIXA_ENTRADA {

        enum class SITUACAO : uint8_t {
            DESCONHECIDO,
            ADICIONAR_CLORO,
            AVISADO_CLORO,
            INICIAR_TRANSFERENCIA,
            AVISADO_TRANSFERENCIA,
            CONCLUIDO,
        };

    void definir(SITUACAO novo);
    SITUACAO atual();
    const __FlashStringHelper* texto(SITUACAO s);

    }
}





//====================================================================================
//                   EVENTOS
//====================================================================================

   namespace EVENTO {
       enum class RESERVATORIO : uint8_t {

    BOMBA_LIGADA,
    BOMBA_DESLIGADA,
    CAIXA_CASA_MEDIA,
    CAIXA_ENTRADA_CLORO,
    ENERGIA_BAIXA

};


    enum class PORTAO : uint8_t {
        PORTAO_ABERTO,
        PORTAO_FECHADO,
        CHUVA_DETECTADA,
        VENTO_FORTE,

};

enum class SOLAR_24V : uint8_t {
          MAQUINA_ROUPA_ACIONADA,
          MAQUINA_ROUPA_DESLIGADA,
          FONTE_USINA_LIGADA,
          LEITURA_VOLTAGEM

};
   };