#include <Arduino.h>
#include <U8g2lib.h>
#include <Wire.h>

U8G2_SSD1306_128X64_NONAME_1_SW_I2C u8g2(U8G2_R0, /* clock=*/ SCL, /* data=*/ SDA, /* reset=*/ U8X8_PIN_NONE);   // All Boards without Reset of the Display

void setup(void) {
    // initialize LED digital pin as an output.
    pinMode(LED_BUILTIN, OUTPUT);
    u8g2.begin();
}

void loop() {

    u8g2.firstPage();
    do {
        u8g2.setFont(u8g2_font_ncenB10_tr);
        u8g2.drawStr(0, 24, "Hello World!");
    } while (u8g2.nextPage());
    // delay(1000);
}