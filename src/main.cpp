#include <Arduino.h>
#include <U8g2lib.h>
#include <WiFi.h>
#include <Wire.h>

// Replace with your network credentials
const char *ssid = "REDACTED";
const char *password = "REDACTED";

int ledState = 0;

U8G2_SSD1306_128X64_NONAME_1_SW_I2C
u8g2(U8G2_R0, /* clock=*/SCL, /* data=*/SDA,
     /* reset=*/U8X8_PIN_NONE); // All Boards without Reset of the Display

void setup(void) {
    Serial.begin(9600);

    // initialize LED digital pin as an output.
    pinMode(LED_BUILTIN, OUTPUT);
    u8g2.begin();

    // Connect to Wi-Fi
    WiFi.begin(ssid, password);
    while (WiFi.status() != WL_CONNECTED) {
        delay(1000);
        Serial.println("Connecting to WiFi...");
    }
}

void loop() {

    u8g2.firstPage();
    do {
        u8g2.setFont(u8g2_font_ncenB10_tr);
        u8g2.drawStr(0, 24, WiFi.localIP().toString().c_str());
    } while (u8g2.nextPage());
}
