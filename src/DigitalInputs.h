#ifndef DIGITALINPUTS_H
#define DIGITALINPUTS_H
#include <Arduino.h>
#include "defines.h"
#define BOUNCE_WITH_PROMPT_DETECTION
//#define BOUNCE_LOCK_OUT
#include <Bounce2.h>
#include "Leds.h"
#include "EnvelopeGenerator.h"

#define MAX_PINS 8
#define BOUNCE_TIME 5 // 5 ms is usually sufficient
typedef void (*CallbackFunctionPtr)(void *, EnvelopeGenerator::EnvelopeStage, EnvelopeGenerator::TrigMode);
class DigitalInputs
{
public:
    DigitalInputs();
    void init();
    void read();
    uint8_t get(uint8_t pin);
    void getGate();
    void getTrig();
    bool hasChanged(uint8_t pin);
    void connectCallback(CallbackFunctionPtr callBack, void *pointer);

    bool getState(uint8_t pin);

    Bounce *digital[MAX_PINS];

private:
    void bounceInit();
    void setSelectorPins(uint8_t j);
    uint8_t muxPin = MUX;
    uint8_t sel0 = S0;
    uint8_t sel1 = S1;
    uint8_t sel2 = S2;
    uint8_t v0 = 0;
    uint8_t v1 = 0;
    uint8_t v2 = 0;

    uint8_t digitalStates[MAX_PINS];

    // The callback provided by the client via connectCallback().
    CallbackFunctionPtr m_callBack;
    // The additional pointer they provided (it's "this").
    void *m_pointer;
};

DigitalInputs::DigitalInputs()
{
    pinMode(muxPin, INPUT);
    pinMode(sel0, OUTPUT);
    pinMode(sel1, OUTPUT);
    pinMode(sel2, OUTPUT);
}

void DigitalInputs::init()
{
    bounceInit();
    digitalWrite(sel0, v0);
    digitalWrite(sel1, v1);
    digitalWrite(sel2, v2);
}
void DigitalInputs::bounceInit()
{
    for (uint8_t i = 0; i < MAX_PINS; i++)
    {
        digital[i] = new Bounce(MUX, BOUNCE_TIME);
    }
}

//select bit
void DigitalInputs::setSelectorPins(uint8_t i)
{
    v0 = bitRead(i, 0);
    v1 = bitRead(i, 1);
    v2 = bitRead(i, 2);
    digitalWrite(sel0, v0);
    digitalWrite(sel1, v1);
    digitalWrite(sel2, v2);
}
void DigitalInputs::read()
{
    for (uint8_t i = 0; i < 8; i++)
    {
        setSelectorPins(i);
        digital[i]->update();
        if (digital[i]->fallingEdge())
        {
            digitalStates[i] = HIGH;
        }
        if (digital[i]->risingEdge())
        {
            digitalStates[i] = LOW;
        }
    }
}

bool DigitalInputs::hasChanged(uint8_t pin)
{
    if (digital[pin]->changed())
        return true;
    else
        return false;
}

uint8_t DigitalInputs::get(uint8_t pin)
{
    return digitalStates[pin];
}

void DigitalInputs::getGate()
{
    if (digital[SW_TRIG]->fallingEdge() || digital[GATE_IN]->fallingEdge())
    {
        m_callBack(m_pointer, EnvelopeGenerator::ENVELOPE_STAGE_ATTACK, EnvelopeGenerator::GATE);
        ledcOn(LED_TRIG);
    }
    else if (digital[SW_TRIG]->risingEdge() || digital[GATE_IN]->risingEdge())
    {
        m_callBack(m_pointer, EnvelopeGenerator::ENVELOPE_STAGE_RELEASE, EnvelopeGenerator::GATE);
        ledcOff(LED_TRIG);
    }
}

void DigitalInputs::getTrig()
{
    if (digital[TRIG_IN]->fallingEdge())
    {
        m_callBack(m_pointer, EnvelopeGenerator::ENVELOPE_STAGE_ATTACK, EnvelopeGenerator::TRIGGER);
        ledcOn(LED_TRIG);
    }
}

void DigitalInputs::connectCallback(CallbackFunctionPtr callBack, void *pointer)
{
    m_callBack = callBack;
    m_pointer = pointer;
}

bool DigitalInputs::getState(uint8_t pin)
{
    return digital[pin]->read();
}
#endif // !DIGITALINPUTS_H