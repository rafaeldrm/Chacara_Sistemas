//====================================================================================
//                   DEPENDÊNCIAS
//====================================================================================

#include "Sequencia_LoRa.h"
#include "DEBUG_MODE.h"

//====================================================================================
//                   VARIÁVEIS INTERNAS
//====================================================================================

namespace LORA
{
namespace AUTENTICACAO
{

//====================================================================================
//                   RESETAR
//====================================================================================
    
void Sequencia_LoRa::Resetar()
{
    sincronizada = false;
    ultima_saida = 0;
    ultima_recebida = 0;
}

//====================================================================================
//                   Esta Sincronizada
//====================================================================================

bool Sequencia_LoRa::EstaSincronizada() const
    {
        return sincronizada;
    }

//====================================================================================
//                   Sincronizar
//====================================================================================

void Sequencia_LoRa::Sincronizar(Sequencia base)
{
    ultima_saida = base;
    ultima_recebida = base;
    sincronizada = true;
}

//====================================================================================
//                   Próxima Saída
//====================================================================================

Sequencia Sequencia_LoRa::ProximaSaida()
{
    if (!sincronizada)
        return 0;

    return ++ultima_saida;
}

//===========================================================s=========================
//                   Ultima Saída
//====================================================================================

Sequencia Sequencia_LoRa::UltimaSaida() const
    {
        return ultima_saida;
    }

//====================================================================================
//                   Classificar Recebida
//====================================================================================
    
ResultadoSequencia Sequencia_LoRa::ClassificarRecebida(Sequencia sequencia)
{
    if (!sincronizada)
        return ResultadoSequencia::INFERIOR;   // não deveria chegar aqui; rejeita por segurança

    if (sequencia > ultima_recebida)
    {
        ultima_recebida = sequencia;
        return ResultadoSequencia::NOVA_MENSAGEM;
    }

    if (sequencia == ultima_recebida)
        return ResultadoSequencia::REPETICAO_INTERNA;

    return ResultadoSequencia::INFERIOR;
}

//====================================================================================
//                   Ultima Recebida
//====================================================================================

Sequencia Sequencia_LoRa::UltimaRecebida() const
    {
        return ultima_recebida;
    }

    }
}