#include "Arduino.h"
#include "Noise.h"

Noise::Noise(NoiseType type)
{
    this->noiseType = type;
}

void Noise::init()
{
    switch (noiseType)
    {
    case NoiseType::NOISE:
        randomSeed(analogRead(27));
        for (uint16_t i = 0; i < TABLE_SIZE; i++)
        {
            noiseTable[i] = generateNoise();
        }
        break;

    case NoiseType::NOISEFILTERED:
        OnePole hpf(20000, SAMPLERATE, OnePole::HIGHPASS);
        randomSeed(analogRead(27));
        for (uint16_t i = 0; i < TABLE_SIZE; i++)
        {
            noiseTable[i] = hpf.process(generateNoise());
            noiseTableInt[i] = static_cast<uint8_t>((noiseTable[i] + 1.0) * 127.0);
        }
        break;
    }
}

void Noise::write()
{
    if (playbackPos < TABLE_SIZE)
    {
        dacWrite(dacPin, noiseTableInt[playbackPos]);
        playbackPos++;
    }
    else
    {
        playbackPos = 0;
        dacWrite(dacPin, noiseTableInt[playbackPos]);
        playbackPos++;
    }
}

inline float Noise::generateNoise()
{
    return (float)((random(0, 256) / 128.0) - 1.0);
}