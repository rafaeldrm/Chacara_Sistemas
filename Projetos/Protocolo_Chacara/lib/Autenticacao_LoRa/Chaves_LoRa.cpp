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


ECO_HIDRO :
    0x70, 0x86, 0x67, 0x85, 0x29, 0x7C, 0x7F, 0x45, 0xB7, 0x84, 0xB0, 0xDB, 0x2B, 0x5A, 0xA4, 0x9B
SOLARIUS24 :
    0x9B, 0xC4, 0x42, 0xAD, 0xBC, 0x74, 0x03, 0x4B, 0xE2, 0x1B, 0xC6, 0xC0, 0xE9, 0x3C, 0xE4, 0x55
GATE_DRIMEL :
    0x7D, 0x40, 0xD7, 0xB2, 0x6A, 0x86, 0x43, 0xE7, 0x1D, 0x0B, 0x50, 0xDD, 0xC7, 0x9E, 0x90, 0x42
RESERVA :
    0x79, 0xA6, 0x83, 0x44, 0x97, 0x93, 0xBF, 0x6E, 0x95, 0xF8, 0x9D, 0x88, 0xBD, 0x27, 0x41, 0x2A

*/

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
                0x70, 0x86, 0x67, 0x85, 0x29, 0x7C, 0x7F, 0x45, 0xB7, 0x84, 0xB0, 0xDB, 0x2B, 0x5A, 0xA4, 0x9B
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