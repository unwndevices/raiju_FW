#ifndef ONEPOLE_H
#define ONEPOLE_H

#include "Arduino.h"
#include "defines.h"

class OnePole
{
public:
    typedef enum
    {
        LOWPASS,
        HIGHPASS,
    } Type;

    OnePole();
    OnePole(float cutoff, float samplingRate, Type type = LOWPASS);
    int setup(float cutoff, float samplingRate, Type type = LOWPASS);
    void setFilter(float cutoff, Type type = LOWPASS);
    float process(float input);

private:
    float _samplingRate;
    int _type;
    float a0, b1, ym1;
    void setFc(float cutoff);
};

#endif