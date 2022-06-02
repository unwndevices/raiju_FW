#ifndef LEDS_H
#define LEDS_H
#include "Arduino.h"
#include "defines.h"

#define LEDS_AMOUNT 5

void ledcInit();
void ledcOn(uint8_t channel);
void ledcOff(uint8_t channel);
void ledcAllOn();
void ledcAllOff();
void ledcAllOtherOff(uint8_t channel);

#endif // LEDS_H