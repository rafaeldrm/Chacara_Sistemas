#include "Executa_Disjuntor.h"
#include <Servo.h>     //Biblioteca externa do Servo
#include "Modulo_Reles.h" // Para ligar o MP1584 antes de movimentar o Servo Motor
#include "Mapa_Pinos_Arduino.h" //serve pra buscar o nome do pino declarado do servo motor : MOVIMENTO_SERVO_MOTOR
#include "DEBUG_MODE.h"
#include "Sistema_Seguranca.h"

//====================================================================================
//                   VARIÁVEIS INTERNAS
//====================================================================================

static Servo meuServo;

//-------------------------- Ângulos que você calibrou ---------------------------------------//

constexpr uint8_t          ANGULO_INICIAL            =       120; // Repouso
constexpr uint8_t          ANGULO_DESLIGAR           =        80; // Empurra para baixo
constexpr uint8_t          ANGULO_LIGAR              =       160; // Puxa para cima

//====================================================================================
//                   SERVO DESLIGA DISJUNTOR
//====================================================================================

//--------------------------  (MODO CALAMIDADE) -------------------------------------------//

void Servo_Motor_Desligar_Disjuntor() {

    SERVO_MOTOR.Ligar();     // Liga alimentação do servo (MP1584) (Relé)
    delay(500);              // Espera estabilizar tensão

    meuServo.attach(pino(ARDUINO_CONEXAO::SERVO::MG996R));

    delay(200);

    meuServo.write(ANGULO_DESLIGAR);
    delay(4000);

    meuServo.detach();

    SERVO_MOTOR.Desligar();  // Corta alimentação do Relé
}


//====================================================================================
//                   SERVO LIGA DISJUNTOR
//====================================================================================

//-------------------------- (MODO RETORNO AO NORMAL, após CALAMIDADE) ----------------------//

void Servo_Motor_Ligar_Disjuntor() {
    Watchdog_Sistema_Atualiza();
    SERVO_MOTOR.Ligar(); //Liga o Relé
    delay(500);

    meuServo.attach(pino(ARDUINO_CONEXAO::SERVO::MG996R));

    delay(200);

    meuServo.write(ANGULO_LIGAR);
    delay(5000);

    meuServo.write(ANGULO_INICIAL);
    delay(300);

    meuServo.detach();
    Watchdog_Sistema_Atualiza();

    SERVO_MOTOR.Desligar(); //Desliga o Relé
}
