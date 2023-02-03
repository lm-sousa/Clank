#include "..\lib\MotorController\MotorController.h"
#include <Arduino.h>
#include <AsyncTCP.h>
#include <ESPAsyncWebServer.h>
#include <U8g2lib.h>
#include <WiFi.h>
#include <Wire.h>

// pins
#define ENA_PIN 15
#define IN1_PIN 2
#define IN2_PIN 4

#define IN3_PIN 5  // verde
#define IN4_PIN 18 // azul
#define ENB_PIN 19 // branco

// PWM channels
#define PWM_CHANNEL1 0
#define PWM_CHANNEL2 1

// Replace with your network credentials
const char *ssid = "REDACTED";
const char *password = "REDACTED";

int ledState = 0;

MotorController Motors(ENA_PIN, IN1_PIN, IN2_PIN, IN3_PIN, IN4_PIN, ENB_PIN,
                       PWM_CHANNEL1, PWM_CHANNEL2);

U8G2_SSD1306_128X64_NONAME_1_SW_I2C
u8g2(U8G2_R0, /* clock=*/SCL, /* data=*/SDA,
     /* reset=*/U8X8_PIN_NONE); // All Boards without Reset of the Display

// Create AsyncWebServer object on port 80
AsyncWebServer server(80);
AsyncWebSocket ws("/ws");

const char index_html[] PROGMEM =
#include "index.html"
    ;

void notifyClients() { ws.textAll(String(ledState)); }

void handleWebSocketMessage(void *arg, uint8_t *data, size_t len) {
    AwsFrameInfo *info = (AwsFrameInfo *)arg;
    if (info->final && info->index == 0 && info->len == len &&
        info->opcode == WS_TEXT) {
        data[len] = 0;
        if (strcmp((char *)data, "toggle") == 0) {
            ledState = !ledState;
            notifyClients();
            return;
        }

        char *btn = strtok((char *)data, ":");
        char *value = strtok(NULL, ":");
        Serial.println(String(btn) + " --> " + String(value));
        if (strcmp(btn, "A") == 0) {
            digitalWrite(LED_BUILTIN, String(value).toInt());
        }
    }
}

void onEvent(AsyncWebSocket *server, AsyncWebSocketClient *client,
             AwsEventType type, void *arg, uint8_t *data, size_t len) {
    switch (type) {
    case WS_EVT_CONNECT:
        Serial.printf("WebSocket client #%u connected from %s\n", client->id(),
                      client->remoteIP().toString().c_str());
        break;
    case WS_EVT_DISCONNECT:
        Serial.printf("WebSocket client #%u disconnected\n", client->id());
        break;
    case WS_EVT_DATA:
        handleWebSocketMessage(arg, data, len);
        break;
    case WS_EVT_PONG:
    case WS_EVT_ERROR:
        break;
    }
}

void initWebSocket() {
    ws.onEvent(onEvent);
    server.addHandler(&ws);
}

String processor(const String &var) {
    Serial.println(var);
    if (var == "STATE") {
        if (ledState) {
            return "ON";
        } else {
            return "OFF";
        }
    }
    return String();
}

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

    initWebSocket();

    // Route for root / web page
    server.on("/", HTTP_GET, [](AsyncWebServerRequest *request) {
        request->send_P(200, "text/html", index_html, processor);
    });

    // Start server
    server.begin();

    Motors.begin();
}

void loop() {
    ws.cleanupClients();

    u8g2.firstPage();
    do {
        u8g2.setFont(u8g2_font_ncenB10_tr);
        u8g2.drawStr(0, 24, WiFi.localIP().toString().c_str());
    } while (u8g2.nextPage());

    Motors.setMotorSpeed(100, 1);
    Motors.setMotorSpeed(100, 2);
}
