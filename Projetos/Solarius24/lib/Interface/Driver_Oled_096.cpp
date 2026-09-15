#include "Driver_Oled_096.h"
#include "Mapa_Pinos_Arduino.h"
#include "DEBUG_MODE.h"
#include "Registrador_Status.h"


//-------------------------- CONSTRUTOR ---------------------------------------//

constexpr uint8_t DISPLAY_OLED = pino(ARDUINO_CONEXAO::I2C_ENDERECO:: DISPLAY_OLED_096);



Driver_Oled_096::Driver_Oled_096()

: display(OLED_WIDTH, OLED_HEIGHT, &Wire, OLED_RESET)
{
    Flags::TornarFalsa(Flags::OLED_DISPONIVEL); //oledDisponivel = false;
    Flags::TornarFalsa(Flags::OLED_ATIVO); //displayAtivo = false; //começa Desligado
}

//====================================================================================
//                   SETUP OLED 096
//====================================================================================

bool Driver_Oled_096::Setup() {

    
    if(!display.begin(SSD1306_SWITCHCAPVCC, DISPLAY_OLED))
    {
        Flags::TornarFalsa(Flags::OLED_DISPONIVEL); //oledDisponivel = false;
        debugErro("Display Com Erro!");
        return false;
    }

    Flags::TornarVerdadeira(Flags::OLED_DISPONIVEL); //oledDisponivel = true;

    Calibrar();
    Ligar();

    display.print(F("INICIADO OLED OK"));
    Atualizar();

    return true;
}

//====================================================================================
//                   OLE DESIPONÍVEL
//====================================================================================

bool Driver_Oled_096::Disponivel()
{
    return Flags::EhVerdadeira(Flags::OLED_DISPONIVEL);
    //return oledDisponivel;
}

//====================================================================================
//                   LIMPA TELA OLED
//====================================================================================

void Driver_Oled_096::Limpar()
{
   if(!Flags::EhVerdadeira(Flags::OLED_DISPONIVEL) || !Flags::EhVerdadeira(Flags::OLED_ATIVO)) return;
    // if(!oledDisponivel || !displayAtivo) return;

    display.clearDisplay();
}

//====================================================================================
//                   ATUALIZAR A TELA OLED
//====================================================================================

void Driver_Oled_096::Atualizar()
{
    if(!Flags::EhVerdadeira(Flags::OLED_DISPONIVEL) || !Flags::EhVerdadeira(Flags::OLED_ATIVO)) return;
    //if(!oledDisponivel || !displayAtivo) return;

    display.display();
}

//====================================================================================
//                   CALIBRAÇÃO DA TELA
//====================================================================================

void Driver_Oled_096::Calibrar()
{
    if(!Flags::EhVerdadeira(Flags::OLED_DISPONIVEL) || !Flags::EhVerdadeira(Flags::OLED_ATIVO)) return;
    // if(!oledDisponivel || !displayAtivo) return;

    display.clearDisplay();
    display.setTextSize(1);
    display.setTextColor(SSD1306_WHITE);
    display.setCursor(0,0);
}

//====================================================================================
//                   LIGA O OLED
//====================================================================================

void Driver_Oled_096::Ligar() {
    if(!Flags::EhVerdadeira(Flags::OLED_DISPONIVEL)) return; // Só precisa saber se o chip existe
    
    Flags::TornarVerdadeira(Flags::OLED_ATIVO); 
    display.ssd1306_command(SSD1306_DISPLAYON);
}

//====================================================================================
//                   DESLIGA O OLED
//====================================================================================

void Driver_Oled_096::Desligar() {
    if(!Flags::EhVerdadeira(Flags::OLED_DISPONIVEL)) return;
    
    Flags::TornarFalsa(Flags::OLED_ATIVO);
    display.ssd1306_command(SSD1306_DISPLAYOFF);
}
//====================================================================================
//                   ESCOLHE A POSIÇÂO DA TELA
//====================================================================================

void Driver_Oled_096::SetCursor(uint8_t x, uint8_t y)
{
    if(!Flags::EhVerdadeira(Flags::OLED_DISPONIVEL) || !Flags::EhVerdadeira(Flags::OLED_ATIVO)) return;
    // if(!oledDisponivel || !displayAtivo) return;

    display.setCursor(x,y);
}

//====================================================================================
//                   ESCREVE NA TELA
//====================================================================================

void Driver_Oled_096::Print(const __FlashStringHelper* texto)
{
    if(!Flags::EhVerdadeira(Flags::OLED_DISPONIVEL) || !Flags::EhVerdadeira(Flags::OLED_ATIVO)) return;
    //if(!oledDisponivel || !displayAtivo) return;

    display.print(texto);
}

//====================================================================================
//                   ESCREVE VALORES FLUTANTES
//====================================================================================

void Driver_Oled_096::PrintValorFormatado(uint16_t valor, uint16_t divisor) 
{
    if(!Flags::EhVerdadeira(Flags::OLED_DISPONIVEL) || !Flags::EhVerdadeira(Flags::OLED_ATIVO)) return;
    //if(!oledDisponivel || !displayAtivo) return;

    uint16_t inteiro = valor / divisor;
    uint16_t decimal = valor % divisor;

    display.print(inteiro);
    display.print(F("."));

    // Descobre quantos dígitos o divisor tem
    uint16_t temp = divisor;
    while (temp > 10) {
        if (decimal < temp / 10) display.print(F("0"));
        temp /= 10;
    }

    display.print(decimal);
}

//====================================================================================
//                  DESENHA LINHAS HORIZONTAIS
//====================================================================================

void Driver_Oled_096::DesenhaLinha(uint8_t x1, uint8_t y1, uint8_t x2, uint8_t y2, uint16_t color)
{
    if(!Flags::EhVerdadeira(Flags::OLED_DISPONIVEL) || !Flags::EhVerdadeira(Flags::OLED_ATIVO)) return;
    //if(!oledDisponivel || !displayAtivo) return;

    display.drawLine(x1, y1, x2, y2, color);
}