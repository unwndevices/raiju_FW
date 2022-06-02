#include "Arduino.h"
#include "Leds.h"

void ledcInit()
{
    for (uint8_t i = 0; i < 5; i++)
    {
        ledcSetup(i, 5000, 8);
    }
    ledcAttachPin(LED_TRIG_PIN, 0);
    ledcAttachPin(LED_ATTACK_PIN, 1);
    ledcAttachPin(LED_DECAY_PIN, 2);
    ledcAttachPin(LED_SUSTAIN_PIN, 3);
    ledcAttachPin(LED_RELEASE_PIN, 4);
    //test initialization
    for (uint8_t i = 0; i < 5; i++)
    {
        for (uint16_t pwm = 0; pwm <= 255; pwm = pwm + 3)
        {
            ledcWrite(i, pwm);
            delay(2);
        }
    }
    for (uint8_t i = 0; i < 5; i++)
    {
        ledcOff(i);
    }
}

void ledcOn(uint8_t channel)
{
    ledcWrite(channel, 255);
}

void ledcOff(uint8_t channel)
{
    ledcWrite(channel, 0);
}

void ledcAllOn()
{
    for (uint8_t i = 0; i < 5; i++)
    {
        ledcOn(i);
    }
}

void ledcAllOff()
{
    for (uint8_t i = 0; i < 5; i++)
    {
        ledcOff(i);
    }
}

void ledcAllOtherOff(uint8_t channel)
{
    for (uint8_t i = 1; i < 5; i++)
    {
        if (i != channel)
        {
            ledcOff(i);
        }
    }
}