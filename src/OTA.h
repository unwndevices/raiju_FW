#ifndef OTA_H
#define OTA_H
#include "Arduino.h"
#include "defines.h"
#include <WiFi.h>
#include "AsyncElegantOTA.h"
#include <AsyncTCP.h>
#include <ESPAsyncWebServer.h>
#include "Interface.h"
#include "Leds.h"
bool otaMode = false;
const char *host = "raiju";
const char *ssid = "UNWN raiju";
const char *password = "pikachu";

AsyncWebServer server(80);
// Set your Static IP address
IPAddress local_IP(192, 168, 1, 1);
// Set your Gateway IP address
IPAddress gateway(192, 168, 1, 1);
IPAddress subnet(255, 255, 0, 0);

void otaStart()
{
    WiFi.softAP(ssid, password);
    WiFi.softAPConfig(local_IP, gateway, subnet);
    AsyncElegantOTA.begin(&server); // Start ElegantOTA
    server.begin();
}

void updateMode()
{
    digitalInputs.read();
    if (digitalInputs.getState(SW_TYPE) == LOW)
    {
        otaMode = true;
        for (uint8_t i = 0; i < 5; i++)
        {
            ledcAllOn();
            delay(100);
            ledcAllOff();
            delay(100);
        }
        ledcAllOn();
        otaStart();
    }
}
#endif // !OTA_H