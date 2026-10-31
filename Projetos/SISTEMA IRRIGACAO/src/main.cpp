/*
    Rafael Drimel
            Março 2025    
    
    **=====================================================================================================================================================**
    ||                                                                                                                                                     ||
    ||                                                                 Arduino MEGA 2560                                                                   ||
    ||                                                            SISTEMA SOLAR 24V - OFF-GRID                                                             ||
    ||                                                                                                                                                     ||
    **=====================================================================================================================================================** 

  Março.2025
  Versão 1.0
  Implementação medir tensão de uma das baterias, caso a voltagem esteja abaixo de 12.5 volts, ativar o relé que ativa a fonte Usina
  Desligar a Fonte Usina quando atingir 14 volts.
  ------------------------------------------------------------------------------------------------------------------------------------------

  Abril 2025
  Versão 2.0 
  Adcionado sensor de temperatura DHT11 no controlador MPPT 
  Acionamento do cooler pra refrigeração do MPPT

  --------------------------------------------------------------------------------------------------------------------------------------------

  Maio 2025
  Versão 3.0  (ATUAL)
  automatização Geladeira e inversor em hipótese emergencial de apagão falta de energia solar e rede elétrica da rua (fonte usina sem alimentação)
  substituição do sensor voltagem 0-25VDC (GND flutuante de uma bateria do conjunto de 2 baterias 12V) tem conflito com GND do regulador de voltagem LM2596
  Implementação INA226 (R010) 0-36V

-----------------------------------------------------------------------------------------------------------------------------------------------
-----------------------------------------------------------------------------------------------------------------------------------------------

  Versão 4.0 (EM BREVE)
  adicionar INA3221 pra leitura por bateria individual - caso de CALAMIDADE NO SISTEMA SOLAR, DESLIGAR TUDO
  adicionar motor de passo pra desligar disjuntor mecanico DC63C - sim já que não encontrei um relé ou módulo que aguente tranco da voltagem alta +/-100V DC e amperagem das placas solares +/- 16 Ampéres
  adicionar LoRa SX1278



NOTA: Farei o possível pra deixar comandos, linhas, codificação a nível usuário, vou evitar de usar comandos complexos tipo array, ou até identação complexa, basta ler e configurar.
*/

/*
    **=======================================================================================================**
    ||                                                                                                       ||
    ||                                            BIBLIOTECAS                                                ||
    ||                                                                                                       ||
    **=======================================================================================================**
*/
#include <Arduino.h> //Biblioteca do Arduino
#include <Wire.h> //Comunicação do IIC
#include <DHT.h> //Biblioteca do Sensor De Temperatura
#include <DHT_U.h> //Biblioteca do Sensor de Temperatura complemento
#define INA226_ADDR 0x40 //voltagem e corrente

/*
    **=======================================================================================================**
    ||                                                                                                       ||
    ||                                            CONEXÕES COM ARDUINO                                       ||
    ||                                                                                                       ||
    **=======================================================================================================**
*/

#define DHTPIN A2  //DHT 11 pino A2
#define cooler_mppt 9  //Relé chinês 
#define Inversor 10  //Relé chinês
#define Geladeira 11  //Relé 1/2 HP 220V 12V
#define Usina 12  //Relé 1/2 HP 220V 12V

/*
    **=======================================================================================================**
    ||                                                                                                       ||
    ||                                            DHT 11                                                     ||
    ||                                                                                                       ||
    **=======================================================================================================**
*/

#define DHTTYPE    DHT11
DHT_Unified dht(DHTPIN, DHTTYPE);

/*
    **=======================================================================================================**
    ||                                                                                                       ||
    ||                                   DIAGRAMA DE MONTAGEM ARDUINO MÓDULOS                                ||
    ||                                                                                                       ||
    **=======================================================================================================**
*/

//ligação dos pinos arduinos alimentação
//GND - Relé do cooler MMPPT gnd / INA 266 gnd e (externo GND)/ Regulador de tensão com saída em 12V (GND) / DHT11 gnd - conectar negativo bateria e todos os comuns negativos.

//12V - Conector JACK arduino Mega2560

//5V -  vcc/ relé do cooler MMPT / vcc / relé inversor vcc / INA226 vcc / DHT11 VCC


//INA226 - SDA  MEGA2560 - 20
//INA226 - SCL -  MEGA2560 - 21
//relé Usina - arduino D12
//relé Geladeira - arduino D11
//relé Inversor - arduino D10
//relé Cooler MMPT - arduino D9
//DHT11 OUT - arduino A2


/*
    **=======================================================================================================**
    ||                                                                                                       ||
    ||                                            VARIÁVEIS GLOBAIS                                          ||
    ||                                                                                                       ||
    **=======================================================================================================**
*/
//medidor de voltagem das baterias (todo o conjunto de baterias como um só)
float volts5 = 100.0, volts4 = 100.0, volts3 = 100.0, volts2 = 100.0, volts1 = 100.0, voltagem_media = 100.0; //os valores eu coloquei alto, pra evitar acionamento indevido na hora de reset do arduino. (aba 3_voltagem_leitura.ino)
//medidor de temperatura do controlador de carga MPPT
int temp_mppt = 1; //valor 0 estado desligado //valor 1 estado ligado - (aba 2_temperatura_mppt.ino)
//Variável do tipo Estado ou Situação, fica setado num valor que se torna uma condição única.
int critico = 0; //valor 0 normal //valor 1 - só quando entra em modo de desligamento dos aparelhos. (aba 4_emergencia.ino)
//Checa se o relé está em pleno funcionamento.
bool FonteUsina = HIGH; //verifica estado do relé da Fonte Usina (aba 4_emergencias.ino)

/*
    **=======================================================================================================**
    ||                                                                                                       ||
    ||                                            REGISTRO DOS VOIDS                                         ||
    ||                                                                                                       ||
    **=======================================================================================================**
*/
void meu_setup();
void temperatura();
void voltagem_leitura();
void emergencia();

/*
    **=======================================================================================================**
    ||                                                                                                       ||
    ||                                            SETUP                                                      ||
    ||                                                                                                       ||
    **=======================================================================================================**
*/

void setup() {

  meu_setup(); // (aba 1_meu_setup.ino) - ativa/posiciona/e varre leituras de módulos comunicação arduino / módulos
  
}

/*
    **=======================================================================================================**
    ||                                                                                                       ||
    ||                                            LOOP                                                       ||
    ||                                                                                                       ||
    **=======================================================================================================**
*/

  void loop() {

    //loop vai entrar de aba em aba e varrer os comandos e informações de forma limpa e fácil de resolver caso haja algum problema ou atualização mesmo.
     temperatura();  //aba DHT11
    //voltagem tem que vir primeiro a qualquer outra aba que tenha informações sobre a leitura voltagem
    voltagem_leitura(); //aba Voltagem
   
    emergencia(); //aba emergência

  }

  void meu_setup() {
  
  Serial.begin(9600); //monitoramento via terminal sketch PC, DEBUG's
  //NOTA tentei usar 115200 porém estava dando interferencias de leitura, uma hora a usina ligou com 12.5V outra com 12.6V, no 9600 está redondo sempre pegando a borda do 12.49v
  
  Wire.begin(); //leitura IC2 ativado
  
  dht.begin(); //Inicializando o sensor DHT11

  //------------------------------------------------------------------------------------------------------------------------------
  //------------------------------------------------------------------------------------------------------------------------------
  //------------------------------------------------------------------------------------------------------------------------------
  //------------------------------------------------------------------------------------------------------------------------------

  //NOTA: ATUALIZAÇÃO DO SISTEMA É OBRIGATÓRIO ALTERAR A VERSÃO NESSA PARTE AQUI ABAIXO DA SERIAL.

  Serial.println("inicializando Sistema Solar versao: 3.0 - MAIO 2025");

   /*
    **=======================================================================================================**
    ||                                                                                                       ||
    ||                              DEFININDO OS PINOS NO ARDUINO                                            ||
    ||                        ATRIBUINDO POSICIONAMENTO DOS PINOS DO ARDUINO                                 ||
    ||                                                                                                       ||
    **=======================================================================================================**
*/

  //definindo posição dos pinos e o como se comportam
  //NOTA: OUTPUT - arduino envia sinal pro módulo / INPUT - arduino recebe sinal do módulo
  pinMode(cooler_mppt, OUTPUT);
  pinMode(Usina, OUTPUT);
  pinMode(Geladeira, OUTPUT);
  pinMode(Inversor, OUTPUT);

  
  //NOTA: HIGH é Armado o módulo, LOW é desligado o módulo
  digitalWrite(Usina, LOW); //Comum e normalmente aberto - o Relé liga e arma a Fonte Usina
  digitalWrite(Geladeira, LOW); //Comum e normalmente fechado - o relé arma e desliga a geladeira
  digitalWrite(Inversor, HIGH); // HIGH é desligado e LOW é Ligado (Relé da China) //Comum e normalmente fechado - o relé arma (LOW) e desliga o inversor
  digitalWrite(cooler_mppt, LOW); // HIGH é desligado e LOW é Ligado (Relé da China) //Comum e normalmente aberto - o relé arma e ativa o cooler
  delay(2000); //se por acaso tiver algum relé armado indevidamente, evitar ruídos pra varredura do resto do sistema.
  //OBS: cooler_mppt vai iniciar armado propositalmente (LOW armado é invertido)

 /*
    **=======================================================================================================**
    ||                                                                                                       ||
    ||                                      TESTANDO e DETALHES                                              ||
    ||                                            DHT11                                                      ||
    ||                                                                                                       ||
    **=======================================================================================================**
*/

  Serial.println(F("DHT11 Verredura de Informações..."));
  // Informar detalhes do sensor de temperatura
  sensor_t sensor;
  dht.temperature().getSensor(&sensor);
  Serial.println(F("------------------------------------"));
  Serial.println(F("Temperatura Sensor"));
  Serial.print(F("Sensor Tipo: ")); Serial.println(sensor.name);
  Serial.print(F("Driver Ver.:  ")); Serial.println(sensor.version);
  Serial.print(F("Exclusivo ID:   ")); Serial.println(sensor.sensor_id);
  Serial.print(F("Max Valor:   ")); Serial.print(sensor.max_value); Serial.println(F("°C"));
  Serial.print(F("Min Valor:   ")); Serial.print(sensor.min_value); Serial.println(F("°C"));
  Serial.print(F("Resolução:  ")); Serial.print(sensor.resolution); Serial.println(F("°C"));
  Serial.println(F("------------------------------------"));
  
  // Informar detalhes do sensor de Umidade -------------------------------------------------------------------------------
  //acho que não tem necessidade de utilização pra esse tipo de sistema, mas deixarei a ideia toda montada e sem necessidade de comenta-la
 
  dht.humidity().getSensor(&sensor);
  Serial.println(F("Umidade Sensor"));
  Serial.print(F("Sensor Tipo: ")); Serial.println(sensor.name);
  Serial.print(F("Driver Ver.:  ")); Serial.println(sensor.version);
  Serial.print(F("Exclusivo ID:   ")); Serial.println(sensor.sensor_id);
  Serial.print(F("Max Valor:   ")); Serial.print(sensor.max_value); Serial.println(F("%"));
  Serial.print(F("Min Valor:   ")); Serial.print(sensor.min_value); Serial.println(F("%"));
  Serial.print(F("Resolução:  ")); Serial.print(sensor.resolution); Serial.println(F("%"));
  Serial.println(F("------------------------------------"));

/*
    **=======================================================================================================**
    ||                                                                                                       ||
    ||                                         CONFIGURAÇÃO                                                  ||
    ||                                            INA226                                                     ||
    ||                                                                                                       ||
    **=======================================================================================================**
*/

  // Setup do INA226 (configuração Registro Opcional)
  Wire.beginTransmission(INA226_ADDR);
  Wire.write(0x00);  // REG_CONFIG
  Wire.write(0x40);  // MSB (padrão 0x4427, aqui config básica)
  Wire.write(0x27);  // LSB
  Wire.endTransmission();
}

//RETORNAR ERRO --------------------------------------------------------------------------------------

void checarErroI2C(byte error) {
  if (error != 0) {
    Serial.print("Erro I2C: ");
    Serial.println(error);
  }
}

/*
    **=======================================================================================================**
    ||                                                                                                       ||
    ||                                      LEITURA DA VOLTAGEM                                              ||
    ||                                            INA226                                                     ||
    ||                                                                                                       ||
    **=======================================================================================================**
*/


  //A Calibragem do INA226 foi realizado com arduino rodando no sistema com o pc espetado na usb dele e monitorando, comparando com a voltagem do INA e do multímetro
 float readBusVoltage() {
  Wire.beginTransmission(INA226_ADDR);
  Wire.write(0x02); // REG_BUS_VOLTAGE
  byte err = Wire.endTransmission();
  if (err != 0) {
    checarErroI2C(err);
    return -1.0; // retorno de erro
  }

  Wire.requestFrom(INA226_ADDR, 2);
  if (Wire.available() < 2) return -1.0;

  int raw = (Wire.read() << 8) | Wire.read();
  return raw * 0.00125;
  
}

//Testes averiguados com precisão, Multímetro marca FOXLUX (alicate amperímetro - FX AA) na escala 200V corrente contínua
//INA: 12.51, Multímetro: 12.5   |  INA: 14.29, Multímetro: 14.2 | INA: 13.74, Multímetro: 13.7

/*
    **=======================================================================================================**
    ||                                                                                                       ||
    ||                                      LEITURA DA AMPERAGEM                                             ||
    ||                                            INA226                                                     ||
    ||                                        (INOPERANTE)                                                   ||
    **=======================================================================================================**
*/

//aqui não tem necessidade de calibração, mas caso seja necessário futuramente eu precisar ler ampéres está ae o código
float readCurrent() {
  Wire.beginTransmission(INA226_ADDR);
  Wire.write(0x04); // REG_CURRENT
  Wire.endTransmission();
  Wire.requestFrom(INA226_ADDR, 2);
  int raw = (Wire.read() << 8) | Wire.read();
  return raw * 0.001; // Conversão genérica (precisa de calibração)
}


void temperatura() {

  //temp_mppt variável serve para evitar ficar entrando no loop sem motivo.
  //o relé como tem orientação invertida HIGH/desligado e LOW/Ligado então é normal ativar o cooler mandando LOW e desligar o cooler com HIGH - relé da china fazer o que.
  //o sistema já entra com o relé ligado cooler funcionando - motivo pra não dar problema erro de variável. fazendo assim uma ventialação forçada inicial e assim estabilizado a variável com as condições impostas.

   sensors_event_t event;
  dht.temperature().getEvent(&event);
  if (isnan(event.temperature)) {
    Serial.println(F("Erro Leitura Sensor"));
  }
  // Retorna o valor em temperatura Celsius
  Serial.print("Temperatura: ");
  Serial.print(event.temperature);
  Serial.println("°C ");
    // Pega valor do sensor da Umidade
  dht.humidity().getEvent(&event);
  if (isnan(event.relative_humidity)) {
    Serial.println(F("Error da Leitura da Umidade"));
  }
  else {
    Serial.print(F("Umidade: "));
    Serial.print(event.relative_humidity);
    Serial.println(F("%"));
  }

  //-------------------------------------------------------------------------------------------------


  if (temp_mppt == 0 && event.temperature >= 35.0) { //35 graus celsius - acredito que é uma temperatura aceitável e assim liga o Cooler para ventilar o controlador de carga MPPT
    Serial.println("Ligando Ventilador");
    digitalWrite(cooler_mppt, LOW); // aqui o led acende do relé e funcionando o Cooler 12V
    temp_mppt = 1; //aqui ele evita de reler a condição
  }

  if (temp_mppt == 1 && event.temperature >= 10.0 && event.temperature <= 26.0) { // 26 graus celsius - desativa o cooler do controlador de carga MPPT
    Serial.println("Desligando Ventilador");
    digitalWrite(cooler_mppt, HIGH); // aqui o led apaga do relé e desativa o cooler 12V
    temp_mppt = 0; //aqui evita de reler a condição
  }
}


void emergencia() {

//NOTA tanto o relé da USINA e GELADEIRA são relés SLA-12VDC-SL-A 30A 250VAC/30VDC
//HIGH é LIGADO e LOW é DESLIGADO
//o Relé do Inversor é um chinês que funciona invertido. HIGH é desligado e LOW é ligado.

//----------------------FONTE USINA ------------------------------------------------------------------------------------------------
//----------------------------------------------------------------------------------------------------------------------------------

    FonteUsina = !digitalRead(Usina); //evitar releituras desnecessária, verifica o estado do relé ligado ou desligado
  
  //AUTOMAÇÃO FONTE USINA
  
  if (FonteUsina == HIGH && voltagem_media <= 12.35) { //12.45V em leitura por bateria
    digitalWrite(Usina, HIGH); //Arma a Fonte Usina
    Serial.println(" Usina ATIVADA");
    delay(1000);
  }
  if (FonteUsina == LOW && voltagem_media >= 14.0) { //14V em leitura por bateria
    digitalWrite(Usina, LOW); //Desarma a Fonte Usina
    Serial.println(" Usina DESATIVADA");
    delay(1000);
  }

  //---------------------PROJETO APAGÃO--------------------------------------------------------------------------------------------------------------------------------------------
  //-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

  //Preservação das baterias e dos aparelhos em geral, caso tenha um apagão a noite/madrugada ou tempo muito ruim de chuva forte, e a fonte Usina (Auxiliar) fique sem alimentação da rua.
  //nível aceitável das baterias 12.4V ou 24.8V
  //aqui a programação só vai varrer se a condição do inteiro *critico*

/*NOTA
    Os relé na posição HIGH vai armar desligando a Geladeira e inversor (a ligação no relé é 'pino Comum' e 'pino normalmente fechado', quando liga o relé o 'pino normalmente aberto' fecha contato com o 'pino comum'.)
    motivo tanto a geladeira quanto o inversor eles passam mais tempos funcionando, e o caso de emergência é algo raríssimo de se acontecer,
    então evita a fadiga dos relés e assim a corrente passa livre nos relés alimentando os aparelhos

 */  
  if (voltagem_media <= 12.15) { //12.3V acho que é um limite bem aceitável - caso fique horas sem retornar a luz da rua ou solar.
    Serial.println("Ativando sistema de emergência");
    digitalWrite(Geladeira, HIGH); //arma o relé pra desligar a geladeira/freezer/internet
    delay(2000); // travar aqui pra dar um tempo para os aparelhos descansrem da forma correta sem queda de tensão errada.
    digitalWrite(Inversor, LOW); //arma o relé pra desligar o inversor
    critico = 1; // aqui faz com que trave a condição de baixo somente pra situação que houve desligamento do sistema forçado.
  }

  if (critico == 1 && voltagem_media >= 13.2) { //antes era 13.6V - mas com alguns testes, 13.1V ou até 13.0V é mais que o suficiente, porém preferi manter em 13.2V, aguenta fácila pancada do arranque da geladeira e freezer.
    Serial.println("Desativando sistema de emergência");
    digitalWrite(Inversor, HIGH); //desarma o relé pra religar o inversor
    delay(15000); //aguardar um bom tempo para que o Inversor se energize, estabilize bem para aguentar a pancada dos picos dos aparelhos.
    digitalWrite(Geladeira, LOW); //desarma o relé pra religar a geladeira/freezer/internet
    critico = 0;
  }

  
}


void voltagem_leitura() {

  float tensaoReferencial = readBusVoltage() / 2; //INA226 fazendo a leitura dos pino VIN e GND
  // float amps = readCurrent(); //INA226 amperagem não aplicado nessa atual vesrão do sistema

  Serial.print("Tensão: ");
  Serial.println(tensaoReferencial); //leitura de voltagem (Tensão) porém dividio por 2 pra facilitar o entendimento por bateria isolada do sistema 24v

 // Serial.print(" V\t Corrente: ");
  //  Serial.print(amps); //leitura de amperagem
  //  Serial.println(" A");
  
  volts5 = volts4;
  delay(1000);
  volts4 = volts3;
  delay(1000);
  volts3 = volts2;
  delay(1000);
  volts2 = volts1;
  delay(1000);
  volts1 = tensaoReferencial;
  delay(1000);

Serial.print("Voltagem 5: "); Serial.println(volts5);
Serial.print("Voltagem 4: "); Serial.println(volts4);
Serial.print("Voltagem 3: "); Serial.println(volts3);
Serial.print("Voltagem 2: "); Serial.println(volts2);
Serial.print("Voltagem 1: "); Serial.println(volts1);

  //poderia fazer isso array mas acho mais simples pra alteração caso precise, e também evitei o uso de millis porque a ideia é 'travar' o sistema pra dar um espaço entre as leituras mesmo.
  //delays de 1 segundo é mais para evitar falsas leituras baixas desnecessárias, tem também a questão de algum aparelho dando puxada em voltagem evitando assim acionamento desnecessário.
  //fora que não precisa de pressa, cair 0.01Volt a mais não mata ninguém.

  voltagem_media = (volts5 + volts4 + volts3 + volts2 + volts1)/5;
  Serial.print("Valor da voltagem média: ");
  Serial.println(voltagem_media);
  Serial.println("===============================================================================================================================================================================");
}
