#include <Arduino.h>

//=== CONFIGURAÇÃO ===
namespace Config {
    static constexpr uint32_t kBaudRate = 9600;
}



//=== TESTE DE HARDWARE ===
namespace HardwareCheck {

    void printReport() {
        Serial.println(F("=== ESP32-S3 Super Mini ==="));
        Serial.print(F("Flash: "));
        Serial.print(ESP.getFlashChipSize() / 1024);
        Serial.println(F(" KB"));
        Serial.print(F("PSRAM: "));
        Serial.print(ESP.getPsramSize() / 1024);
        Serial.println(F(" KB"));
        Serial.print(F("Heap livre: "));
        Serial.print(ESP.getFreeHeap() / 1024);
        Serial.println(F(" KB"));
        Serial.print(F("CPU: "));
        Serial.print(ESP.getCpuFreqMHz());
        Serial.println(F(" MHz"));
    }

}



//=== SETUP E LOOP ===
void setup() {
    Serial.begin(Config::kBaudRate);
    delay(2000);  // a USB nativa precisa de um tempo pra reconectar
    HardwareCheck::printReport();
}



void loop() {
    delay(1000);
}