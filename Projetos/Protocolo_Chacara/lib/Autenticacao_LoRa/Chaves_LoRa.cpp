//====================================================================================
//                   DEPENDÊNCIAS
//====================================================================================

#include "Chaves_LoRa.h"
#include "DEBUG_MODE.h"


/* abre o powershell, (aperte windows + r) digite powershell aperte enter e cole o código abaixo e aperte enter


$rng = [System.Security.Cryptography.RandomNumberGenerator]::Create()
foreach ($nome in 'ECO_HIDRO','SOLARIUS24','GATE_DRIMEL','RESERVA') {
    $b = New-Object byte[] 16
    $rng.GetBytes($b)
    "$nome :"
    "    " + (($b | ForEach-Object { '0x{0:X2}' -f $_ }) -join ', ')
}

//====================================================================================
//                   VARIÁVEIS INTERNAS
//====================================================================================

namespace LORA
{
    namespace AUTENTICACAO
    {
        namespace
        {
        // PLACEHOLDER INTENCIONAL.
        //
        // Não usar uma chave real aqui.
        // A chave definitiva será inserida somente depois que a estrutura
        // do protocolo estiver fechada.
        
            constexpr uint8_t CHAVE_DO_SISTEMA[TAMANHO_CHAVE_LORA] =
            {
      //ocultado a chave secreta
            };
        }

    const Chave& ObterChave()
    {
        static const Chave chave =
        {
            {
                CHAVE_DO_SISTEMA[0],  CHAVE_DO_SISTEMA[1],
                CHAVE_DO_SISTEMA[2],  CHAVE_DO_SISTEMA[3],
                CHAVE_DO_SISTEMA[4],  CHAVE_DO_SISTEMA[5],
                CHAVE_DO_SISTEMA[6],  CHAVE_DO_SISTEMA[7],
                CHAVE_DO_SISTEMA[8],  CHAVE_DO_SISTEMA[9],
                CHAVE_DO_SISTEMA[10], CHAVE_DO_SISTEMA[11],
                CHAVE_DO_SISTEMA[12], CHAVE_DO_SISTEMA[13],
                CHAVE_DO_SISTEMA[14], CHAVE_DO_SISTEMA[15]
            }
        };
            return chave;
        }

    }
}