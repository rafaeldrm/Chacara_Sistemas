//====================================================================================
//                   DEPENDÊNCIAS
//====================================================================================

#include "Leitura_Voltagem.h"
#include "Driver_INA226.h"
#include "DEBUG_MODE.h"

//====================================================================================
//                   VARIÁVEIS INTERNAS
//====================================================================================

static DadosLeituraVoltagem _dados;

static constexpr uint8_t TAMANHO_BUFFER = 5;
static constexpr uint16_t INTERVALO_LEITURA = 2500;
static constexpr uint8_t DIVISOR_ESCALA = 20; //se for 200 (resultado 124 (12.4), se for em 20 (resultado 1245 (12.45))

static uint16_t _buffer[TAMANHO_BUFFER];
static uint32_t _soma = 0;
static uint8_t  _indice = 0;

static uint32_t _ultimoMillis = 0;

//====================================================================================
//                   SETUP
//====================================================================================

bool Leitura_Voltagem_Inicializa()
{
    debugSetupArquivo();
    debugSetupTexto("LEITURA :: INA226 :: INICIANDO ::", NEGRITO | MAGENTA | LINHA);

    _dados =
    {
        .atual = 0,
        .anterior = 0,
        .media = 0,
        .novo_dado = false,

        .estado = LEITURA::NAO_PRONTA,
        .falhas_seguidas = 0
    };

    _soma = 0;
    _indice = 0;

    for (uint8_t i = 0; i < TAMANHO_BUFFER; i++)
        _buffer[i] = 0;

    _ultimoMillis = 0;

    debugOk("INA226 pronto");

    return true;
}

//====================================================================================
//                   LOOP
//====================================================================================

void Leitura_Voltagem_Atualiza()
{
    if (millis() - _ultimoMillis < INTERVALO_LEITURA)
        return;

    _ultimoMillis = millis();

    uint32_t tensaoBruta;

//================ LEITURA DO DRIVER =================//

    if (!Driver_INA226_Ler_Mili_Volts(&tensaoBruta))
    {
        debugErro("INA226 leitura falhou");

        _dados.estado    = LEITURA::NAO_PRONTA;
        _dados.novo_dado = false;

        // FALHOU: soma +1 (com trava em 255 pra não dar a volta)
        if (_dados.falhas_seguidas < 255)
            _dados.falhas_seguidas++;

        return;
    }

 // DEU CERTO: zera a contagem de falhas
    _dados.falhas_seguidas = 0;

    //================ ESCALA =================//
    // Pacote em mV → por bateria com 4 dígitos (1245 = 12.45V)
    // Ex.: 24900 mV / 20 = 1245   (÷2 bancada→bateria, ÷10 mV→centivolt)
    uint16_t tensaoBanco = tensaoBruta / DIVISOR_ESCALA;

    _dados.atual = tensaoBanco;

    //================ BUFFER =================//
    _soma -= _buffer[_indice];
    _buffer[_indice] = tensaoBanco;
    _soma += tensaoBanco;

    _indice++;

    if (_indice >= TAMANHO_BUFFER)
    {
        _indice = 0;

         if (_dados.estado != LEITURA::PRONTA)
        {
             _dados.estado = LEITURA::PRONTA;
            debugOk("INA226 média estabilizada");
        }
    }

    //================ MÉDIA =================//

    if (_dados.estado == LEITURA::PRONTA)
    {
        _dados.media = _soma / TAMANHO_BUFFER;

        _dados.novo_dado = true;
    }
}

//====================================================================================
//                   ACESSO AOS DADOS
//====================================================================================

const DadosLeituraVoltagem& Leitura_Voltagem_Obter_Dados()
{
    return _dados;
}

bool Leitura_Voltagem_Tem_Novo_Dado()
{
    return _dados.novo_dado;
}

void Leitura_Voltagem_Consumir()
{
     _dados.novo_dado = false;
}

void Leitura_Voltagem_Valor_Anterior()
{
   //================ HISTÓRICO =================//
    _dados.anterior = _dados.atual;

}