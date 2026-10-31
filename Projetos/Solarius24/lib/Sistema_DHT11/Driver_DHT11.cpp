//====================================================================================
//                   DEPENDÊNCIAS
//====================================================================================

#include "Driver_DHT11.h"
#include "Mapa_Pinos_Arduino.h"
#include "DEBUG_MODE.h" 

//====================================================================================
//                   VARIÁVEIS INTERNAS
//====================================================================================

   static constexpr uint16_t TIMEOUT_LOOP   = 1000; //tempo de espera para o loop
   
   static constexpr uint16_t TIMEOUT_ESPERA = 100; //tempo de espera para a espera

   static constexpr uint8_t PINO_DHT11 = pino(ARDUINO_CONEXAO::SENSOR_DIGITAL::TEMPERATURA_MPPT_DHT11); //pino do sensor DHT11


//====================================================================================
//                   SETUP
//====================================================================================

void Driver_DHT11_Inicializa() 
{

    debugSetupArquivo();
    debugSetupTexto("DRIVER :: DHT11 :: INICIALIZANDO :: ", NEGRITO | VERDE | LINHA);
    debugVariavel("DHT11 :: PINO ARDUINO :: ", PINO_DHT11, NEGRITO, VERDE, AMARELO);

}

//====================================================================================
//                   ACESSO AOS DADOS (AGUARDA NÍVEL)
//====================================================================================

bool Driver_DHT11_AguardaNivel(bool nivel, uint16_t timeout_us) //função para aguardar o nível do sinal
{

    uint32_t inicio = micros(); //variável inicio salva o micro segundos do inicio da função

    while (digitalRead(PINO_DHT11) != nivel)  //loop para aguardar o nível do sinal
    {
        if ((micros() - inicio) > timeout_us) //se o tempo de espera for maior que o timeout, retorna false
            return false; //retorna false se o tempo de espera for maior que o timeout
    }

    return true; //retorna true se o nível do sinal for atingido

}

//====================================================================================
//                  ACESSO AOS DADOS (TEMPERATURA / UMIDADE)
//====================================================================================

bool Driver_DHT11_Ler(uint8_t &temperatura, uint8_t &umidade) //função para ler a temperatura e a umidade
{

    static uint8_t dados[5] = {0, 0, 0, 0, 0}; //inicia os bits internos resetados
    static uint16_t contador; //variável contador salva o contador do sensor DHT11

    //===============================================================================
    // START SINAL
    //===============================================================================

    for (uint8_t i = 0; i < 5; i++) {
    dados[i] = 0;
    }

    digitalWrite(PINO_DHT11, HIGH);
    delay(1);

    pinMode(PINO_DHT11, OUTPUT); //configura o pino do sensor DHT11 como saída  
    digitalWrite(PINO_DHT11, LOW);
    delay(20); //espera 20ms

    pinMode(PINO_DHT11, INPUT_PULLUP); //configura o pino do sensor DHT11 como entrada pull-up

    //===============================================================================
    // HANDSHAKE
    //===============================================================================

    if (!Driver_DHT11_AguardaNivel(LOW,  TIMEOUT_ESPERA)) return false; //aguarda o nível LOW
    if (!Driver_DHT11_AguardaNivel(HIGH, TIMEOUT_ESPERA)) return false; //aguarda o nível HIGH
    if (!Driver_DHT11_AguardaNivel(LOW,  TIMEOUT_ESPERA)) return false; //aguarda o nível LOW

    //===============================================================================
    // LEITURA DOS 40 BITS
    //===============================================================================

    for (uint8_t i = 0; i < 40; i++) //loop para ler os 40 bits
    {
        // Aguarda início do pulso (LOW → HIGH)
        contador = 0; //zera o contador
        while (digitalRead(PINO_DHT11) == LOW) //loop para aguardar o nível LOW
        {
            if (++contador > TIMEOUT_LOOP) //se o contador for maior que o timeout, retorna false
                return false; //retorna false se o contador for maior que o timeout
        }

        // Pequeno atraso para amostragem
        for (uint8_t w = 0; w < 10; w++) //loop para ler os 10 bits
            digitalRead(PINO_DHT11); //lê o pino do sensor DHT11

        // Leitura do bit
        if (digitalRead(PINO_DHT11) == HIGH) //se o pino do sensor DHT11 for HIGH
        {
            dados[i / 8] |= (1 << (7 - (i % 8))); //salva o bit no array dados
        }

        // Aguarda fim do HIGH (vale para bit 0 e 1)
        contador = 0; //zera o contador
        while (digitalRead(PINO_DHT11) == HIGH) //loop para aguardar o nível HIGH
        {
            if (++contador > TIMEOUT_LOOP) //se o contador for maior que o timeout, retorna false
                return false; //retorna false se o contador for maior que o timeout
        }
    }

    //===============================================================================
    //  VERIFICADOR DE DADOS 
    //===============================================================================

    uint8_t verificador = dados[0] + dados[1] + dados[2] + dados[3]; //verifica o verificador dos dados

    if (verificador != dados[4]) //se o verificador for diferente dos dados, retorna false
        return false; //retorna false se o verificador for diferente dos dados

    //===============================================================================
    // SAÍDA
    //===============================================================================

    umidade     = dados[0]; //salva a umidade no array dados
    temperatura = dados[2]; //salva a temperatura no array dados

    return true; //retorna true se a leitura foi feita com sucesso

}