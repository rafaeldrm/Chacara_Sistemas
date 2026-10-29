//====================================================================================
//                   DEPENDÊNCIAS
//====================================================================================

#include "Estado_Sistema_Drimel_System.h"
#include "DEBUG_MODE.h"

//====================================================================================
//                   VARIÁVEIS INTERNAS
//====================================================================================

struct EstadosSistemaDrimelSystem
{

    SISTEMA_DRIMEL_SYSTEM::ESTADO sistemaDrimelSystemModo;
    IA::PERSONALIDADE::PERSONAGEM personalidadeIA;
    IA::SENTIMENTOS::AVALIACAO sentimentosIA;
    IA::ESTAGIOS::ESTADO estagiosIA;
    TELA::MENU::OPCAO telaMenu;
    TELA::MANUTENCAO::ETAPAS telaManutencao;
    ECO_HIDRO::CAIXA_ENTRADA::SITUACAO ecoHidroCaixaEntrada;
    EVENTO::RESERVATORIO::MENSAGEM eventoReservatorio;
    EVENTO::SOLAR_24V::MENSAGEM eventoSolar24v;
    EVENTO::PORTAO::MENSAGEM eventoPortao;

};


static EstadosSistemaDrimelSystem estado =
{
    SISTEMA_DRIMEL_SYSTEM::ESTADO::NORMAL,
    IA::PERSONALIDADE::PERSONAGEM::KAMYLLA,
    IA::SENTIMENTOS::AVALIACAO::NENHUM,
    IA::ESTAGIOS::ESTADO::APRENDENDO,
    TELA::MENU::OPCAO::NORMAL,
    TELA::MANUTENCAO::ETAPAS::NENHUM,
    ECO_HIDRO::CAIXA_ENTRADA::SITUACAO::DESCONHECIDO,
    EVENTO::RESERVATORIO::MENSAGEM::NENHUM,
    EVENTO::SOLAR_24V::MENSAGEM::NENHUM,
    EVENTO::PORTAO::MENSAGEM::NENHUM,

};

//====================================================================================
//                   SISTEMA MODOS
//====================================================================================

namespace SISTEMA_DRIMEL_SYSTEM
{
    void definir(ESTADO novo)
    {
        if (estado.sistemaDrimelSystemModo == novo) return;
        estado.sistemaDrimelSystemModo = novo;
        debugVariavel("ESTADO sistemaDrimelSystemModo : ", texto(novo), NENHUM, VERDE, AMARELO);
    }

    ESTADO atual()
    {
        return estado.sistemaDrimelSystemModo;
    }

    const __FlashStringHelper* texto(ESTADO s)
    {
        switch(s)
        {
            case ESTADO::NORMAL:                                         return F("NORMAL");
            case ESTADO::FERIAS:                                         return F("FERIAS");
            case ESTADO::BLOQUEADO:                                      return F("BLOQUEADO");
            case ESTADO::MANUTENCAO:                                     return F("MANUTENCAO");
            case ESTADO::FESTA:                                          return F("FESTA");
            case ESTADO::EMERGENCIA:                                     return F("EMERGENCIA");
            case ESTADO::NOTURNO:                                        return F("NOTURNO");   

            default:                return F("DESCONHECIDO");
        }
    }
}

//====================================================================================
//                   IA 
//====================================================================================

namespace IA
{

    //====================================================================================
    //                   PERSONALIDADE IA
    //====================================================================================
    
    
    namespace PERSONALIDADE
    {
        void definir(PERSONAGEM novo)
        {
            if (estado.personalidadeIA == novo) return;
            estado.personalidadeIA = novo;
            debugVariavel("PERSONAGEM personalidadeIA : ", texto(novo), NENHUM, VERDE, AMARELO);
        }

        PERSONAGEM atual()
        {
            return estado.personalidadeIA;
        }

        const __FlashStringHelper* texto(PERSONAGEM p)
        {
            switch(p)
            {
                case PERSONAGEM::KAMYLLA:                                               return F("KAMYLLA");
                case PERSONAGEM::SPYRO:                                                 return F("SPYRO");

                default:                return F("DESCONHECIDO");
            }
        }
    }

    //====================================================================================
    //                   SENTIMENTOS IA
    //====================================================================================
    
        namespace SENTIMENTOS
    {
        void definir(AVALIACAO novo)
        {
            if (estado.sentimentosIA == novo) return;
            estado.sentimentosIA = novo;
            debugVariavel("SENTIMENTOS sentimentosIA : ", texto(novo), NENHUM, VERDE, AMARELO);
        }

        AVALIACAO atual()
        {
            return estado.sentimentosIA;
        }

        const __FlashStringHelper* texto(AVALIACAO s)
        {
            switch(s)
            {
                case AVALIACAO::NENHUM:                                                 return F("NENHUM");
                case AVALIACAO::BOM:                                                    return F("BOM");
                case AVALIACAO::RUIM:                                                   return F("RUIM");
                case AVALIACAO::NEUTRO:                                                 return F("NEUTRO");

                default:                return F("DESCONHECIDO");
            }
        }
    }

    //====================================================================================
    //                   ESTAGIOS IA
    //====================================================================================
    
       namespace ESTAGIOS
    {
        void definir(ESTADO novo)
        {
            if (estado.estagiosIA == novo) return;
            estado.estagiosIA = novo;
            debugVariavel("ESTADO estagiosIA : ", texto(novo), NENHUM, VERDE, AMARELO);
        }

        ESTADO atual()
        {
            return estado.estagiosIA;
        }

        const __FlashStringHelper* texto(ESTADO e)
        {
            switch(e)
            {
                case ESTADO::APRENDENDO:                                              return F("APRENDENDO");
                case ESTADO::TESTANDO:                                                return F("TESTANDO");
                case ESTADO::ESTUDANDO:                                               return F("ESTUDANDO");
                case ESTADO::DECISAO:                                                 return F("DECISAO");
                case ESTADO::AUTOMATICO:                                              return F("AUTOMATICO");
                case ESTADO::SIMULANDO:                                               return F("SIMULANDO");

                default:                return F("DESCONHECIDO");
            }
        }
    }
}

//====================================================================================
//                   DISPLAY TFT 4.0
//====================================================================================

namespace TELA {

    //====================================================================================
    //                   MENU
    //====================================================================================

  namespace MENU
    {
        void definir(OPCAO novo)
        {
            if (estado.telaMenu == novo) return;
            estado.telaMenu = novo;
            debugVariavel("TELA telaMenu : ", texto(novo), NENHUM, VERDE, AMARELO);
        }

        OPCAO atual()
        {
            return estado.telaMenu;
        }

        const __FlashStringHelper* texto(OPCAO e)
        {
            switch(e)
            {
                case OPCAO::NORMAL:                                              return F("NORMAL");
                case OPCAO::MANUTENCAO:                                          return F("MANUTENCAO");

                default:                return F("DESCONHECIDO");
            }
        }
    }

    //====================================================================================
    //                   MANUTENÇÃO
    //====================================================================================

  namespace MANUTENCAO
    {
        void definir(ETAPAS novo)
        {
            if (estado.telaManutencao == novo) return;
            estado.telaManutencao = novo;
            debugVariavel("TELA telaManutencao : ", texto(novo), NENHUM, VERDE, AMARELO);
        }

        ETAPAS atual()
        {
            return estado.telaManutencao;
        }

        const __FlashStringHelper* texto(ETAPAS e)
        {
            switch(e)
            {
                case ETAPAS::NENHUM:                                             return F("NENHUM");
                case ETAPAS::ATUALIZAR:                                          return F("ATUALIZAR");
                case ETAPAS::DESLIGAMENTO_SEGURO:                               return F("DESLIGAMENTO_SEGURO");

                default:                return F("DESCONHECIDO");
            }
        }
    }
}

//====================================================================================
//                   EVENTOS
//====================================================================================

 namespace EVENTO {
        namespace RESERVATORIO
        {

             void definir(MENSAGEM novo)
        {
            if (estado.eventoReservatorio == novo) return;
            estado.eventoReservatorio = novo;
            debugVariavel("EVENTO reservatorio : ", texto(novo), NENHUM, VERDE, AMARELO);
        }

        MENSAGEM atual()
        {
            return estado.eventoReservatorio;
        }

        const __FlashStringHelper* texto(MENSAGEM s)
        {
            switch(s)
            {
                case MENSAGEM::NENHUM:                                                   return F("NENHUM");
                case MENSAGEM::BOMBA_LIGADA:                                             return F("BOMBA_LIGADA");
                case MENSAGEM::BOMBA_DESLIGADA:                                          return F("BOMBA_DESLIGADA");
                case MENSAGEM::CAIXA_CASA_MEDIA:                                         return F("CAIXA_CASA_MEDIA");
                case MENSAGEM::CAIXA_ENTRADA_CLORO:                                      return F("CAIXA_ENTRADA_CLORO");
                case MENSAGEM::ENERGIA_BAIXA:                                            return F("ENERGIA_BAIXA");

                default:                return F("DESCONHECIDO");
            }
        }
    }

        namespace PORTAO
        {

             void definir(MENSAGEM novo)
        {
            if (estado.eventoPortao == novo) return;
            estado.eventoPortao = novo;
            debugVariavel("EVENTO portao : ", texto(novo), NENHUM, VERDE, AMARELO);
        }

        MENSAGEM atual()
        {
            return estado.eventoPortao;
        }

        const __FlashStringHelper* texto(MENSAGEM s)
        {
            switch(s)
            {
                case MENSAGEM::NENHUM:                                                    return F("NENHUM");
                case MENSAGEM::PORTAO_ABERTO:                                             return F("PORTAO_ABERTO");
                case MENSAGEM::PORTAO_FECHADO:                                            return F("PORTAO_FECHADO");
                case MENSAGEM::CHUVA_DETECTADA:                                           return F("CHUVA_DETECTADA");
                case MENSAGEM::VENTO_FORTE:                                               return F("VENTO_FORTE");

                default:                return F("DESCONHECIDO");
            }
        }
    }
        namespace SOLAR_24V
        {


            
             void definir(MENSAGEM novo)
        {
            if (estado.eventoSolar24v == novo) return;
            estado.eventoSolar24v = novo;
            debugVariavel("EVENTO solar24v : ", texto(novo), NENHUM, VERDE, AMARELO);
        }

        MENSAGEM atual()
        {
            return estado.eventoSolar24v;
        }

        const __FlashStringHelper* texto(MENSAGEM s)
        {
            switch(s)
            {
                case MENSAGEM::NENHUM:                                                    return F("NENHUM");
                case MENSAGEM::MAQUINA_ROUPA_ACIONADA:                                    return F("MAQUINA_ROUPA_ACIONADA");
                case MENSAGEM::FONTE_USINA_LIGADA:                                        return F("FONTE_USINA_LIGADA");
                case MENSAGEM::LEITURA_VOLTAGEM:                                           return F("LEITURA_VOLTAGEM");

                default:                return F("DESCONHECIDO");
            }
        }
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

  namespace CAIXA_ENTRADA
    {
        void definir(SITUACAO novo)
        {
            if (estado.ecoHidroCaixaEntrada == novo) return;
            estado.ecoHidroCaixaEntrada = novo;
            debugVariavel("CAIXA ENTRADA ecoHidroCaixaEntrada : ", texto(novo), NENHUM, VERDE, AMARELO);
        }

        SITUACAO atual()
        {
            return estado.ecoHidroCaixaEntrada;
        }

        const __FlashStringHelper* texto(SITUACAO s)
        {
            switch(s)
            {
                case SITUACAO::DESCONHECIDO:                                       return F("DESCONHECIDO");
                case SITUACAO::ADICIONAR_CLORO:                                    return F("ADICIONAR_CLORO");
                case SITUACAO::AVISADO_CLORO:                                      return F("AVISADO_CLORO");
                case SITUACAO::CLORO_CONFIRMADO:                                   return F("CLORO_CONFIRMADO");
                case SITUACAO::INICIAR_TRANSFERENCIA:                              return F("INICIAR_TRANSFERENCIA");
                case SITUACAO::AVISADO_TRANSFERENCIA:                              return F("AVISADO_TRANSFERENCIA");
                case SITUACAO::CONCLUIDO:                                          return F("CONCLUIDO");

                default:                return F("DESCONHECIDO");
            }
        }
    }
}