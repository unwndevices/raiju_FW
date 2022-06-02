#ifndef ANALOGINPUTS_h
#define ANALOGINPUTS_h
#include <Arduino.h>
#include "defines.h"
#include "systemTimer.h"
#include "ResponsiveAnalogRead.h"

#define ADC_INPUTS 9

class AnalogInputs
{
public:
    void init()
    {
        for (uint8_t i = 0; i < ADC_INPUTS; i++)
        {
            analog[i] = new ResponsiveAnalogRead(adcPins[i], true, 0.5);

            analog[i]->setAnalogResolution(4096);
            analog[i]->setActivityThreshold(0.9);
            analog[i]->disableEdgeSnap();
        }
    }
    void read()
    {
        for (uint8_t i = 0; i < ADC_INPUTS; i++)
        {
            analog[i]->update();
            analog_data[i] = analog[i]->getValue();
        }
    };

    uint16_t get(uint8_t pin)
    {
        return analog_data[pin];
    };

    uint8_t adcPins[ADC_INPUTS] = {POT_ATTACK,
                                   POT_DECAY,
                                   POT_SUSTAIN,
                                   POT_RELEASE,
                                   CV_ATTACK,
                                   CV_DECAY,
                                   CV_SUSTAIN,
                                   CV_RELEASE,
                                   POT_SHAPE};

    ResponsiveAnalogRead *analog[ADC_INPUTS];

private:
    uint16_t analog_data[ADC_INPUTS];
};

#endif