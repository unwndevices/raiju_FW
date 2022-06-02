#ifndef AUDIO_H
#define AUDIO_H
#include "Arduino.h"
#include "defines.h"
#include "systemTimer.h"
#include "Noise.h"
#include "DAC8552.h"
#include "EnvelopeGenerator.h"
#include "Interface.h"
#include "PolyBLEPOscillator.h"

EnvelopeGenerator envelope;
DAC8552 dac(DAC_SS);
Noise noise(Noise::NoiseType::NOISEFILTERED);
PolyBLEPOscillator osc;

void setOsc(Oscillator::OscillatorMode mode = Oscillator::OSCILLATOR_MODE_SINE)
{
    osc.setMode(mode);
}

void audioInit()
{
    dac.begin();
    noise.init();
}

void audioUpdate()
{
    if (getAudioTimer() == 1)
    {
        dac.setValue(0, envelope.nextSample16());
        noise.write();
    }
    setAudioTimer(0);
}

#endif // !AUDIO_H