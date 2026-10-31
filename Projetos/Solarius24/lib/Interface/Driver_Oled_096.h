#pragma once

#include <Arduino.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

class  Driver_Oled_096
{
private:

    Adafruit_SSD1306 display;

    static constexpr int8_t OLED_WIDTH = 128;
    static constexpr int8_t OLED_HEIGHT = 64;
    static constexpr uint8_t OLED_RESET = -1;

public:

   Driver_Oled_096();

    bool Setup();

    bool Disponivel();

    void Ligar();

    void Desligar();

    void Limpar();

    void Atualizar();

    void Calibrar();

    void SetCursor(uint8_t x, uint8_t y);

    void Print(const __FlashStringHelper* texto);

    void PrintValorFormatado(uint16_t valor, uint16_t divisor);

    void DesenhaLinha(uint8_t x1, uint8_t y1, uint8_t x2, uint8_t y2, uint16_t color);

};

extern Driver_Oled_096 Display_Oled;