#include <Arduino.h>
#include <U8g2lib.h>
#include <Wire.h>
#include "..\lib\MotorController\MotorController.h"

//pins
#define ENA_PIN 15
#define IN1_PIN 2
#define IN2_PIN 4 

#define IN3_PIN 5 //verde
#define IN4_PIN 18 //azul
#define ENB_PIN 19 //branco

//PWM channels
#define PWM_CHANNEL1 0
#define PWM_CHANNEL2 1

U8G2_SSD1306_128X64_NONAME_1_SW_I2C u8g2(U8G2_R0, /* clock=*/ SCL, /* data=*/ SDA, /* reset=*/ U8X8_PIN_NONE);   // All Boards without Reset of the Display
MotorController Motors(ENA_PIN, IN1_PIN, IN2_PIN, IN3_PIN, IN4_PIN, ENB_PIN, PWM_CHANNEL1, PWM_CHANNEL2);

void setup(void) {
    // initialize LED digital pin as an output.
    pinMode(LED_BUILTIN, OUTPUT);
    u8g2.begin();
    Motors.begin();
}

void loop() {
    Motors.setMotorSpeed(100, 2);
    delay(50);
}