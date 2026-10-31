#include "Interface_Oled_096.h"
#include "Driver_Oled_096.h"
#include "DEBUG_MODE.h"

constexpr uint8_t OLED_WIDTH = 128;
constexpr uint8_t OLED_HEIGHT = 64;


void AtualizarValoresDisplay(
    uint16_t temperatura, //DHT11
    uint16_t tensaoAC, //ZMPT101B
    uint16_t tensaoACMax, //ZMPT101B
    uint16_t tensaoACMin, //ZMPT101B 
    uint16_t tensaoBancada1_Bateria1, //INA3221 #1 (Canal 1)
    uint16_t tensaoBancada1_Bateria2, //INA3221 #1 (Canal 2)
    uint16_t tensaoBancada2_Bateria1, //INA3221 #2 (Canal 1)
    uint16_t tensaoBancada2_Bateria2, //INA3221 #2 (Canal 2)
    uint16_t tensaoBateria, //INA226
    uint16_t correnteMaquina //SCT013
) {
    if (!Display_Oled.Disponivel()) return;

    Display_Oled.Limpar();
    Display_Oled.Calibrar();    
    Display_Oled.Print(F("TEMP.:  "));
    Display_Oled.PrintValorFormatado(temperatura, 10); //leitura DHT11 (temperatura)
    Display_Oled.Print(F(" C"));

    Display_Oled.SetCursor(0,10);
    Display_Oled.Print(F("A/C: "));
    Display_Oled.PrintValorFormatado(tensaoAC, 100); //leitura do ZMPT101B atual
    Display_Oled.Print(F(" > "));
    Display_Oled.PrintValorFormatado(tensaoACMax, 100); //leitura do ZMPT101B máxima
    Display_Oled.Print(F(" < "));
    Display_Oled.PrintValorFormatado(tensaoACMin, 100); //leitura do ZMPT101B mínima

    Display_Oled.SetCursor(0,20);
    Display_Oled.Print(F("B1: "));
    Display_Oled.PrintValorFormatado(tensaoBancada1_Bateria1, 100); //INA3221 #1 bateria (cheia 24V), convertido para 12V
    Display_Oled.Print(F(" / "));
    Display_Oled.PrintValorFormatado(tensaoBancada1_Bateria2, 100); //INA3221 #1 bateria (parcial 12V)

    Display_Oled.SetCursor(0,30);
    Display_Oled.Print(F("B2: "));
    Display_Oled.PrintValorFormatado(tensaoBancada2_Bateria1, 100); //INA3221 #2 bateria (cheia 24V), convertido para 12V
    Display_Oled.Print(F(" / "));
    Display_Oled.PrintValorFormatado(tensaoBancada2_Bateria2, 100);//INA3221 #2 bateria (parcial 12V)

    Display_Oled.SetCursor(0,40);
    Display_Oled.Print(F("Voltagem: "));
    Display_Oled.PrintValorFormatado(tensaoBateria, 100); //INA226 convertido para 12V

    Display_Oled.SetCursor(0,50);
    Display_Oled.Print(F("Maq. Lavar: "));
    Display_Oled.PrintValorFormatado(correnteMaquina, 100); //valor do SCT013 (amperagem do disjuntor)
    Display_Oled.Print(F(" A"));

    Display_Oled.Atualizar();
}