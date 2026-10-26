//====================================================================================
//                   DEPENDÊNCIAS
//====================================================================================

#include "Diagnostico_INA226.h"
#include "Driver_INA226.h"
#include "DEBUG_MODE.h"

#include "Cronometro.h"
#include "Tempo.h"

//====================================================================================
//                   CONFIGURAÇÕES
//====================================================================================

// 1) Falhas de leitura seguidas (Driver devolveu false) até declarar ERRO
static constexpr uint8_t  LIMITE_FALHAS_LEITURA = 5;

// 2) Valor impossível (escala de 4 dígitos: 1245 = 12.45V por bateria)
static constexpr uint16_t TENSAO_MINIMA_ABSURDA = 500;    //  5.00V
static constexpr uint16_t TENSAO_MAXIMA_ABSURDA = 2000;   // 20.00V

// 3) Chip vivo? (pergunta o ID ao INA226)
static constexpr uint32_t TEMPO_CHECAR_CHIP_NORMAL = TEMPO::MINUTOS(5);
static constexpr uint32_t TEMPO_CHECAR_CHIP_ERRO   = TEMPO::SEGUNDOS(10);  // em erro, confere rápido pra perceber a recuperação

// 4) Travado: valor EXATAMENTE igual por muito tempo (só aviso, nunca vira ERRO sozinho)
static constexpr uint32_t TEMPO_SUSPEITA_TRAVADO   = TEMPO::HORAS(1);

//====================================================================================
//                   ESTADO
//====================================================================================

static StatusDiagnosticoINA226 _diag;

// Causas de ERRO (qualquer uma acesa = ERRO)
static bool _erro_falhas  = false;
static bool _erro_absurdo = false;
static bool _erro_chip    = false;

static CRONOMETRO::Temporizador _cronometroChip;
static CRONOMETRO::Temporizador _cronometroTravado;

//====================================================================================
//                   INIT
//====================================================================================

void Diagnostico_INA226_Inicializa()
{
    _diag =
    {
        .estado = SENSOR::OK,
        .estado_anterior = SENSOR::OK,

        .ultima_tensao = 0,

        .contador_erros = 0,
        .suspeita_travado = false,

        .erro_lora_enviado = false,
        .mostrar_erro_oled = false,

        .sensor_online = false,
        .erro_sensor = false,

        .solicitar_recuperacao = false,

        // EVENTOS
        .evento_recuperado = false,
        .evento_erro_novo = false,
    };

    _erro_falhas  = false;
    _erro_absurdo = false;
    _erro_chip    = false;

    _cronometroChip.Cronometro_Iniciar();
    _cronometroTravado.Cronometro_Iniciar();

    debugOk("Diagnóstico INA226 pronto");
}

//====================================================================================
//                   CHECAGEM 1 :: FALHAS DE LEITURA SEGUIDAS
//====================================================================================

static void Checar_Falhas_Leitura(const DadosLeituraVoltagem& leitura)
{
    bool falhou = (leitura.falhas_seguidas >= LIMITE_FALHAS_LEITURA);

    if (falhou && !_erro_falhas)
        debugErro("INA226 SEM LEITURA (falhas seguidas)");

    _erro_falhas = falhou;
}

//====================================================================================
//                   CHECAGEM 2 :: VALOR IMPOSSÍVEL
//====================================================================================

static void Checar_Valor_Impossivel(uint16_t tensao)
{
    bool absurdo = (tensao < TENSAO_MINIMA_ABSURDA || tensao > TENSAO_MAXIMA_ABSURDA);

    if (absurdo && !_erro_absurdo)
        debugErro("INA226 VALOR IMPOSSIVEL");

    _erro_absurdo = absurdo;
}

//====================================================================================
//                   CHECAGEM 3 :: CHIP ESTÁ VIVO?
//====================================================================================

static void Checar_Chip_Vivo()
{
    uint32_t intervalo = _erro_chip ? TEMPO_CHECAR_CHIP_ERRO : TEMPO_CHECAR_CHIP_NORMAL;

    if (!_cronometroChip.Cronometro_Ciclo_Continuo(intervalo))
        return;

    bool vivo = Driver_INA226_Disponivel();

    if (!vivo && !_erro_chip)
        debugErro("INA226 NAO RESPONDE (chip)");

    _erro_chip = !vivo;
}

//====================================================================================
//                   CHECAGEM 4 :: SUSPEITA DE TRAVADO (SÓ AVISO)
//====================================================================================

static void Checar_Travado(uint16_t tensao)
{
    // Valor mudou: tudo certo, recomeça a contar
    if (tensao != _diag.ultima_tensao)
    {
        _cronometroTravado.Cronometro_Iniciar();
        _diag.suspeita_travado = false;
        return;
    }

    // Valor igual por muito tempo: levanta o aviso (uma vez)
    if (_cronometroTravado.Cronometro_Dispara_Uma_Vez(TEMPO_SUSPEITA_TRAVADO))
    {
        _diag.suspeita_travado = true;
        debugAviso("INA226 SUSPEITA: valor igual ha muito tempo");
    }
}

//====================================================================================
//                   ESTADO FINAL
//====================================================================================

static void Definir_Estado()
{
    _diag.erro_sensor = (_erro_falhas || _erro_absurdo || _erro_chip);

    if (_diag.erro_sensor)
        _diag.estado = SENSOR::ERRO;

    else if (_diag.suspeita_travado)
        _diag.estado = SENSOR::INSTAVEL;

    else
        _diag.estado = SENSOR::OK;

    _diag.sensor_online         = (_diag.estado != SENSOR::ERRO);
    _diag.solicitar_recuperacao = (_diag.estado == SENSOR::ERRO);
    _diag.mostrar_erro_oled     = (_diag.estado != SENSOR::OK);
}

//====================================================================================
//                   EVENTOS (duram um ciclo)
//====================================================================================

static void Gerar_Eventos()
{
    // ERRO NOVO
    if (_diag.estado_anterior != SENSOR::ERRO && _diag.estado == SENSOR::ERRO)
    {
        _diag.evento_erro_novo = true;
        _diag.contador_erros++;
        debugAviso("INA226 ENTROU EM ERRO");
    }

    // RECUPERADO
    if (_diag.estado_anterior == SENSOR::ERRO && _diag.estado != SENSOR::ERRO)
    {
        _diag.evento_recuperado = true;
        debugOk("INA226 RECUPERADO");
    }

    // LORA (UMA VEZ POR ERRO)
    if (_diag.estado == SENSOR::ERRO && !_diag.erro_lora_enviado)
    {
        debugAviso("LORA ERRO INA226");
        _diag.erro_lora_enviado = true;
    }

    // LIBERA NOVO ENVIO QUANDO NORMALIZAR
    if (_diag.estado == SENSOR::OK)
    {
        _diag.erro_lora_enviado = false;
    }
}

//====================================================================================
//                   LOOP
//====================================================================================

void Diagnostico_INA226_Atualiza(const DadosLeituraVoltagem& leitura)
{
    //============================================================
    // RESET DAS FLAGS VOLÁTEIS
    //============================================================

    _diag.estado_anterior       = _diag.estado;
    _diag.solicitar_recuperacao = false;
    _diag.evento_recuperado     = false;
    _diag.evento_erro_novo      = false;

    //============================================================
    // CHECAGENS QUE RODAM SEMPRE (não dependem de dado novo)
    //============================================================

    Checar_Falhas_Leitura(leitura);
    Checar_Chip_Vivo();

    //============================================================
    // CHECAGENS QUE SÓ FAZEM SENTIDO COM DADO NOVO
    //============================================================

    if (leitura.estado == LEITURA::PRONTA && leitura.novo_dado)
    {
        Checar_Valor_Impossivel(leitura.media);
        Checar_Travado(leitura.media);

        _diag.ultima_tensao = leitura.media;
    }

    //============================================================
    // RESULTADO
    //============================================================

    Definir_Estado();
    Gerar_Eventos();
}

//====================================================================================
//                   GET
//====================================================================================

const StatusDiagnosticoINA226& Diagnostico_INA226_Obter_Status()
{
    return _diag;
}