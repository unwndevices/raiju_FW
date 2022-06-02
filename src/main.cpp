#include <Arduino.h>
#include "Interface.h"
#include "Audio.h"
#include "Leds.h"
#include "OTA.h"
#include "tasks.h"

void setup()
{
  audioInit();
  ledcInit();
  Serial.end();
  initAudioTimer();
  taskInit();
  //  initInterfaceTimer();
  //  interfaceInit();
  delay(100);
  updateMode();
}

void loop()
{
  if (otaMode == false)
  {
    audioUpdate();
    //    interfaceUpdate();
  }
}