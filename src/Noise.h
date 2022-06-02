#ifndef NOISE_H
#define NOISE_H
#include "Arduino.h"
#include "defines.h"
#include "OnePole.h"

#define TABLE_SIZE 4096

class Noise
{
public:
    typedef enum
    {
        NOISE = 0,
        NOISEFILTERED
    } NoiseType;

    Noise(NoiseType type = NOISE);
    void init();
    void write();

private:
    NoiseType noiseType;
    uint8_t dacPin = ESP_DAC;
    inline float generateNoise();
    float noiseTable[TABLE_SIZE];
    uint8_t noiseTableInt[TABLE_SIZE];
    uint16_t playbackPos = 0;
};

#endif