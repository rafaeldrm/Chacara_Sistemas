//====================================================================================
//                   DEPENDÊNCIAS
//====================================================================================

#include "Leitura_Baterias.h"
#include "DEBUG_MODE.h"

#include "Driver_INA3221.h"

//====================================================================================
//                   VARIÁVEIS INTERNAS
//====================================================================================

static constexpr int32_t FATOR_DIVISOR = 11;

static constexpr uint8_t EMA_PESO_ANTIGO = 9;
static constexpr uint8_t EMA_DIVISOR     = 10;

static constexpr uint8_t CICLOS_ESTABILIZACAO = 4;

static constexpr int32_t INA3221_LSB_uV = 8000; // 8mV

//====================================================================================
//                   CONSTRUTORES DAS STRUCTS
//====================================================================================

static DadosBancada bancada1;
static DadosBancada bancada2;

static PackBaterias pack;

static uint8_t ciclos_validos = 0;

static bool ciclo_completo = false;

//====================================================================================
//                   CONVERSÃO
//====================================================================================

static int32_t Converter_mV(int16_t raw)
{

return (raw * INA3221_LSB_uV) / 1000; // mV

}

//====================================================================================
//                   PROCESSAMENTO
//====================================================================================

static bool Processar_Bancada(Driver_INA3221& sensor, DadosBancada &b)
{
    int16_t raw1, raw2;

   if (!sensor.Ler_Canal(1, &raw1) ||
    !sensor.Ler_Canal(2, &raw2))
{
    b.erro = true;
    return false;
}

    int32_t ch1 = Converter_mV(raw1);
    int32_t ch2 = Converter_mV(raw2);

    int32_t total    = ch1 * FATOR_DIVISOR;
    int32_t inferior = ch2;
    int32_t superior = total - inferior;

    if (b.erro)
    {
        b.voltagemTotal_mV    = total;
        b.bateriaInferior_mV  = inferior;
        b.bateriaSuperior_mV  = superior;
        b.erro = false;
    }
    else
    {
        b.voltagemTotal_mV    = (b.voltagemTotal_mV * EMA_PESO_ANTIGO + total) / EMA_DIVISOR;
        b.bateriaInferior_mV  = (b.bateriaInferior_mV * EMA_PESO_ANTIGO + inferior) / EMA_DIVISOR;
        b.bateriaSuperior_mV  = (b.bateriaSuperior_mV * EMA_PESO_ANTIGO + superior) / EMA_DIVISOR;
    }

    b.delta_mV = (b.bateriaSuperior_mV > b.bateriaInferior_mV)
    ? (b.bateriaSuperior_mV - b.bateriaInferior_mV)
    : (b.bateriaInferior_mV - b.bateriaSuperior_mV);

    return true;
}

//====================================================================================
//                   SETUP
//====================================================================================

void Leitura_Baterias_Inicializa()
{

    debugSetupArquivo();
    debugSetupDivisao(TEXTO_AMARELO, "LEITURA BATERIAS INICIALIZANDO!", TEXTO_MAGENTA, "-", 120);

    bancada1 = {0,0,0,0,true};
    bancada2 = {0,0,0,0,true};

    for (uint8_t i = 0; i < 4; i++) {
        pack.celula[i] = 0;
        pack.celula_anterior[i] = 0;
    }

    pack.maior = 0;
    pack.menor = 0;
    pack.delta = 0;

    pack.erro = true;
    pack.pronta = false;
    pack.novo_dado = false;

    ciclos_validos = 0;

    debugOk("Leitura baterias pronta, Variáveis Limpas");
}

//====================================================================================
//                   LOOP
//====================================================================================

void Leitura_Baterias_Loop()
{
    static bool alternar = false;
    alternar = !alternar;

    //================ LEITURA INTERCALADA =================//

    if (alternar) {
        Processar_Bancada(SENSOR_INA3221_B1, bancada1);
    } else {
        Processar_Bancada(SENSOR_INA3221_B2, bancada2);
    }

    //================ PACK COMPLETO =================//

    if (!bancada1.erro && !bancada2.erro)
    {

        for (uint8_t i = 0; i < 4; i++) {
            pack.celula_anterior[i] = pack.celula[i];
        }

        pack.celula[0] = bancada1.bateriaInferior_mV;
        pack.celula[1] = bancada1.bateriaSuperior_mV;
        pack.celula[2] = bancada2.bateriaInferior_mV;
        pack.celula[3] = bancada2.bateriaSuperior_mV;

        pack.maior = pack.celula[0];
        pack.menor = pack.celula[0];


        for (uint8_t i = 1; i < 4; i++) {
            if (pack.celula[i] > pack.maior) pack.maior = pack.celula[i];
            if (pack.celula[i] < pack.menor) pack.menor = pack.celula[i];
        }

            if (!alternar) {
        ciclo_completo = true;
    }

        pack.delta = pack.maior - pack.menor;

        pack.erro = false;

        if (!pack.pronta)
        {
            ciclos_validos++;

            if (ciclos_validos >= CICLOS_ESTABILIZACAO)
            {
                pack.pronta = true;
                debugOk("PACK BATERIAS ESTABILIZADO");
            }
        }


        if (pack.pronta && ciclo_completo)
{
    pack.novo_dado = true;
    ciclo_completo = false;
}
    }
    else
    {
        pack.erro = true;
        ciclos_validos = 0;
        pack.pronta = false;
        ciclo_completo = false; 
    }
}

//====================================================================================
//                   ACESSO AOS DADOS
//====================================================================================

const DadosBancada& Leitura_Baterias_Obter_Bancada1() { return bancada1; }
const DadosBancada& Leitura_Baterias_Obter_Bancada2() { return bancada2; }

const PackBaterias& Leitura_Baterias_Obter_Pack() { return pack; }

bool Leitura_Baterias_TemNovoDado() { return pack.novo_dado; }

void Leitura_Baterias_Consumir() { pack.novo_dado = false; }