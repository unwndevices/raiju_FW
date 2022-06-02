
#ifndef INTERFACE_H
#define INTERFACE_H
#include <Arduino.h>
#include "DigitalInputs.h"
#include "AnalogInputs.h"
#include "systemTimer.h"
#include "Audio.h"

#define LOW_SPEED 100.0
#define MEDIUM_SPEED 5.0
#define HIGH_SPEED 1.0

DigitalInputs digitalInputs;
AnalogInputs analogInputs;
double speedRange = 0.0;

uint8_t digitalBuffer[MAX_PINS];
float cvValues[4] = {0};

void interfaceInit();
void interfaceUpdate();
void envSpeed();
void envelopeValUpdate(uint8_t pin, EnvelopeGenerator::EnvelopeStage stage);
void valueUpdateRoutine();

void interfaceInit()
{
    analogInputs.init();
    digitalInputs.init();
    digitalInputs.connectCallback(EnvelopeGenerator::staticCallbackFunction, &envelope);
    // check interface at boot
    digitalInputs.read();
    for (uint8_t i = 0; i < MAX_PINS; i++)
    {
        digitalBuffer[i] = digitalInputs.get(i);
    }
    envSpeed();
}

void interfaceUpdate()
{
    if (getInterfaceTimer() == 1)
    {
        digitalInputs.read();
        for (uint8_t i = 0; i < MAX_PINS; i++)
        {
            if (digitalInputs.hasChanged(i) == true)
            {
                digitalBuffer[i] = digitalInputs.get(i);
            }
        }
        if (digitalInputs.hasChanged(TOG_HIGH) == true || digitalInputs.hasChanged(TOG_LOW) == true)
        {
            envSpeed(); //updates the env speed logic switch
        }
        digitalInputs.getGate();
        digitalInputs.getTrig();

        analogInputs.read();
        valueUpdateRoutine();
        setInterfaceTimer(0);
    }
}

void envSpeed()
{
    if (digitalBuffer[TOG_HIGH] == HIGH && digitalBuffer[TOG_LOW] == LOW)
    {
        speedRange = HIGH_SPEED;
    }
    else if (digitalBuffer[TOG_HIGH] == LOW && digitalBuffer[TOG_LOW] == HIGH)
    {
        speedRange = LOW_SPEED;
    }
    else
        speedRange = MEDIUM_SPEED;
}

void envelopeValUpdate(uint8_t pin, EnvelopeGenerator::EnvelopeStage stage)
{
    double value = (double)(analogInputs.get(pin));
    if (pin == 0 || pin == 1)
    {
        value = ((4095.0 - value) / 4095.0) + 0.01;
    }
    else if (pin == 0 || pin == 1)
    {
        value = ((4095.0 - value) / 4095.0);
    }
    else
    {
        value = (value / 4095.0);
    }
    if (pin != 2)
    {
        value = (value + cvValues[pin]) * speedRange;
    }
    else
    {
        value = constrain((value + cvValues[pin]), 0.0, 1.0);
    }
    envelope.setStageValue(stage, value);
}

void valueUpdateRoutine()
{
    for (uint8_t i = 0; i < ADC_INPUTS; i++)
    {
        if (analogInputs.analog[i]->hasChanged() == true)
        {
            if (i <= 3)
            {
                envelopeValUpdate(i, static_cast<EnvelopeGenerator::EnvelopeStage>(i + 1));
            }
            if (i >= 4 && i <= 7)
            {
                cvValues[i - 4] = (double)(analogInputs.get(i) / 4095.0);
            }
        }
    }
}

#endif // !INTERFACE_H