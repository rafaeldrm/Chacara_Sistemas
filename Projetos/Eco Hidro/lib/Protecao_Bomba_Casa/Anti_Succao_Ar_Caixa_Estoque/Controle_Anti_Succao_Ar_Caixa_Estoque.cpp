//====================================================================================
//                   DEPENDÊNCIAS
//====================================================================================

#include "Controle_Anti_Succao_Ar_Caixa_Estoque.h"
#include "DEBUG_MODE.h"

#include "Estados_Sistema_Eco_Hidro.h"
#include "Leitura_Boias.h"

//====================================================================================
//                   VARIÁVEIS INTERNAS
//====================================================================================

static bool boiaCaixaEstoqueMinimaVazia = false;

//====================================================================================
//                   SETUP
//====================================================================================

void Controle_Anti_Succao_Ar_Caixa_Estoque_Inicializa()
{
        debugSetupDivisao(COR::AMARELO, "CONTROLE ANTI SUCCAO AR CAIXA ESTOQUE INICIALIZANDO", COR::MAGENTA, "=", 0);
}

//====================================================================================
//                   LOOP
//====================================================================================

void Controle_Anti_Succao_Ar_Caixa_Estoque_Atualiza()
{   
     boiaCaixaEstoqueMinimaVazia = !Leitura_Boias_Esta_Ativa(REGISTRO_BOIAS::ESTOQUE_MINIMA);

     // ESTOQUE ficando vazio é ruim pra válvula de retenção da bomba ficar puxando ar.
    if (boiaCaixaEstoqueMinimaVazia) {
        DESLIGAMENTO_BOMBA::definir(DESLIGAMENTO_BOMBA::ESTADO::ESTOQUE_MINIMO);
        BOMBA_CASA::definir(BOMBA_CASA::ESTADO::DESLIGANDO_BOMBA);
        return;
    }



}
