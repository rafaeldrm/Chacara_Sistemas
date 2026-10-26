//====================================================================================
//                   DEPENDÊNCIAS
//====================================================================================

#include "Mediador_Unidade_Potencia.h"
#include "DEBUG_MODE.h"

#include "Estados_Sistema_Eco_Hidro.h"

//====================================================================================
//                   REGRA DA CASA (LEIA ANTES DE MEXER)
//====================================================================================

/*
    QUEM PEDE (Mediador Casa_Torre, Mediador Reservatório, futuramente o Portão):
        1) só pede se POTENCIA == REPOUSO  (ou seja, ninguém está usando o inversor)
        2) pede assim:  POTENCIA::definir(APARELHO)  +  APARELHO::definir(LIBERADO)
        3) quando terminar, manda o aparelho para DESLIGANDO_BOMBA e NÃO mexe em mais nada.

    QUEM EXECUTA (SÓ este arquivo):
        - liga o inversor quando o aparelho pedido estiver em AGUARDA_LIBERACAO
        - manda o aparelho ligar quando o inversor estiver LIGADO
        - desliga o inversor quando o aparelho voltar para REPOUSO
        - devolve POTENCIA para REPOUSO (fila liberada para o próximo pedido)

    RESUMO DA NOVELA (cada seta é um "loop" do Arduino):

        POTENCIA = BOMBA_X, BOMBA_X = LIBERADO      (quem pediu)
        BOMBA_X = AGUARDA_LIBERACAO                 (Gerencia da bomba)
        UNIDADE = ACIONAMENTO_RELES                 (este arquivo)
        UNIDADE = AGUARDANDO_ESTABILIZAR → LIGADO   (Controle da Unidade)
        BOMBA_X = LIGANDO_BOMBA → BOMBA_LIGADA      (este arquivo + Controle da bomba)
        ...bomba trabalhando...
        BOMBA_X = DESLIGANDO_BOMBA → REPOUSO        (quem pediu + Controle da bomba)
        UNIDADE = DESLIGANDO → REPOUSO              (este arquivo)
        POTENCIA = DESLIGANDO → REPOUSO             (este arquivo)  <- fila livre
*/

//====================================================================================
//                   SETUP
//====================================================================================

void Mediador_Unidade_Potencia_Inicializa()
{
        debugSetupDivisao(COR::AMARELO, "MEDIADOR UNIDADE POTENCIA INICIALIZANDO", COR::MAGENTA, "=", 45);
}

//====================================================================================
//                   AUXILIAR: APARELHO ESCOLHIDO ESTÁ AGUARDANDO LIBERAÇÃO?
//====================================================================================

static bool Mediador_Unidade_Potencia_Aparelho_Aguardando_Liberacao()
{
    switch (POTENCIA::atual())
    {
        case POTENCIA::SELECIONADO::BOMBA_CASA:

            return BOMBA_CASA::atual() == BOMBA_CASA::ESTAGIOS::AGUARDA_LIBERACAO;

        //========================================================

        case POTENCIA::SELECIONADO::BOMBA_12V:

            return BOMBA_12V::atual() == BOMBA_12V::ESTAGIOS::AGUARDA_LIBERACAO;

        //========================================================

        default:

            return false;
    }
}

//====================================================================================
//                   AUXILIAR: APARELHO ESCOLHIDO JÁ VOLTOU PARA REPOUSO?
//====================================================================================

static bool Mediador_Unidade_Potencia_Aparelho_Em_Repouso()
{
    switch (POTENCIA::atual())
    {
        case POTENCIA::SELECIONADO::BOMBA_CASA:

            return BOMBA_CASA::atual() == BOMBA_CASA::ESTAGIOS::REPOUSO;

        //========================================================

        case POTENCIA::SELECIONADO::BOMBA_12V:

            return BOMBA_12V::atual() == BOMBA_12V::ESTAGIOS::REPOUSO;

        //========================================================

        default:

            return false; // REPOUSO / DESLIGANDO / PORTAO: não é "bomba terminando".
    }
}

//====================================================================================
//                   TRAVA DE SEGURANÇA: UM APARELHO POR VEZ
//====================================================================================

// Em teoria nunca dispara (quem pede só pede com POTENCIA livre).
// Fica aqui como cinto de segurança: se dois aparelhos estiverem ativos,
// só o escolhido sobrevive.

static void Mediador_Unidade_Potencia_Trava_Um_Aparelho_Por_Vez()
{
    switch (POTENCIA::atual())
    {
        case POTENCIA::SELECIONADO::BOMBA_CASA:

            if (BOMBA_12V::atual() != BOMBA_12V::ESTAGIOS::REPOUSO
            &&  BOMBA_12V::atual() != BOMBA_12V::ESTAGIOS::DESLIGANDO_BOMBA)
            {
                debugAviso("Trava Potencia: Bomba 12V desligada a força");
                BOMBA_12V::definir(BOMBA_12V::ESTAGIOS::DESLIGANDO_BOMBA);
            }

        break;

        //========================================================

        case POTENCIA::SELECIONADO::BOMBA_12V:

            if (BOMBA_CASA::atual() != BOMBA_CASA::ESTAGIOS::REPOUSO
            &&  BOMBA_CASA::atual() != BOMBA_CASA::ESTAGIOS::DESLIGANDO_BOMBA)
            {
                debugAviso("Trava Potencia: Bomba Casa desligada a força");
                BOMBA_CASA::definir(BOMBA_CASA::ESTAGIOS::DESLIGANDO_BOMBA);
            }

        break;

        //========================================================

        default:

        break;
    }
}

//====================================================================================
//                   ACIONA A UNIDADE (INVERSOR + RELÉS 12V)
//====================================================================================

static void Mediador_Unidade_Potencia_Aciona_Unidade()
{
    if (UNIDADE_POTENCIA::atual() != UNIDADE_POTENCIA::ESTAGIOS::REPOUSO)
        return;

    if (!Mediador_Unidade_Potencia_Aparelho_Aguardando_Liberacao())
        return;

    // Quando a potência vai para um aparelho, as luzes saem do caminho (consumo).
    LUZ_ACESSO::definir(LUZ_ACESSO::SITUACAO::DESLIGANDO);
    LUZ_ESTACIONAMENTO::definir(LUZ_ESTACIONAMENTO::SITUACAO::DESLIGANDO);

    UNIDADE_POTENCIA::definir(UNIDADE_POTENCIA::ESTAGIOS::ACIONAMENTO_RELES);
}

//====================================================================================
//                   UNIDADE LIGADA E ESTÁVEL: MANDA O APARELHO LIGAR
//====================================================================================

static void Mediador_Unidade_Potencia_Liga_Aparelho()
{
    if (UNIDADE_POTENCIA::atual() != UNIDADE_POTENCIA::ESTAGIOS::LIGADO)
        return;

    switch (POTENCIA::atual())
    {
        case POTENCIA::SELECIONADO::BOMBA_CASA:

            if (BOMBA_CASA::atual() == BOMBA_CASA::ESTAGIOS::AGUARDA_LIBERACAO)
            {
                BOMBA_CASA::definir(BOMBA_CASA::ESTAGIOS::LIGANDO_BOMBA);
            }

        break;

        //========================================================

        case POTENCIA::SELECIONADO::BOMBA_12V:

            if (BOMBA_12V::atual() == BOMBA_12V::ESTAGIOS::AGUARDA_LIBERACAO)
            {
                BOMBA_12V::definir(BOMBA_12V::ESTAGIOS::LIGANDO_BOMBA);
            }

        break;

        //========================================================
        // PORTAO: quando o Gate Drimel existir, entra aqui.
        //========================================================

        default:

        break;
    }
}

//====================================================================================
//                   APARELHO TERMINOU: DESLIGA A UNIDADE
//====================================================================================

static void Mediador_Unidade_Potencia_Desliga_Unidade()
{
    if (!Mediador_Unidade_Potencia_Aparelho_Em_Repouso())
        return;

    const auto unidade = UNIDADE_POTENCIA::atual();

    // Pedido cancelado antes de o inversor ligar: só devolve a vez.
    if (unidade == UNIDADE_POTENCIA::ESTAGIOS::REPOUSO)
    {
        POTENCIA::definir(POTENCIA::SELECIONADO::REPOUSO);
        return;
    }

    // Já está desligando, não repete.
    if (unidade == UNIDADE_POTENCIA::ESTAGIOS::DESLIGANDO)
        return;

    UNIDADE_POTENCIA::definir(UNIDADE_POTENCIA::ESTAGIOS::DESLIGANDO);
    POTENCIA::definir(POTENCIA::SELECIONADO::DESLIGANDO); // fila trancada até o inversor apagar
}

//====================================================================================
//                   INVERSOR APAGOU: LIBERA A FILA
//====================================================================================

static void Mediador_Unidade_Potencia_Libera_Fila()
{
    if (POTENCIA::atual() == POTENCIA::SELECIONADO::DESLIGANDO
    &&  UNIDADE_POTENCIA::atual() == UNIDADE_POTENCIA::ESTAGIOS::REPOUSO)
    {
        POTENCIA::definir(POTENCIA::SELECIONADO::REPOUSO);
    }
}

//====================================================================================
//                   LOOP
//====================================================================================

void Mediador_Unidade_Potencia_Atualiza()
{
    Mediador_Unidade_Potencia_Trava_Um_Aparelho_Por_Vez();

    Mediador_Unidade_Potencia_Aciona_Unidade();

    Mediador_Unidade_Potencia_Liga_Aparelho();

    Mediador_Unidade_Potencia_Desliga_Unidade();

    Mediador_Unidade_Potencia_Libera_Fila();
}