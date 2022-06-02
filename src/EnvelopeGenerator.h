#ifndef ENVELOPEGENERATOR_H
#define ENVELOPEGENERATOR_H
#include <Arduino.h>
#include "Leds.h"
#include <cmath>

class EnvelopeGenerator
{
public:
    enum EnvelopeStage
    {
        ENVELOPE_STAGE_OFF = 0,
        ENVELOPE_STAGE_ATTACK,
        ENVELOPE_STAGE_DECAY,
        ENVELOPE_STAGE_SUSTAIN,
        ENVELOPE_STAGE_RELEASE,
        kNumEnvelopeStages
    };
    enum TrigMode
    {
        GATE = 0,
        TRIGGER
    };
    enum TrigOutType
    {
        TRIG_ATTACK = 0,
        TRIG_DECAY,
        TRIG_SUSTAIN,
        TRIG_RELEASE
    };

    void enterStage(EnvelopeStage newStage, TrigMode mode);
    void setTrigOutType(TrigOutType type);
    uint16_t nextTrigOut(uint8_t trigTime);
    double nextSample();
    uint16_t nextSample16();
    void setSampleRate(double newSampleRate);
    inline EnvelopeStage getCurrentStage() const { return currentStage; };
    const double minimumLevel;

    void setStageValue(EnvelopeStage stage, double value);

    EnvelopeGenerator() : minimumLevel(0.0001),
                          currentStage(ENVELOPE_STAGE_OFF),
                          currentLevel(minimumLevel),
                          currentExpLevel(minimumLevel),
                          currentInvLevel(1.0 - minimumLevel),
                          multiplier(1.0),
                          increment(0.0),
                          currentShape(0.0),
                          sampleRate(45454.5),
                          currentSampleIndex(0),
                          nextStageSampleIndex(0)

    {
        stageValue[ENVELOPE_STAGE_OFF] = 0.0;
        stageValue[ENVELOPE_STAGE_ATTACK] = 0.01;
        stageValue[ENVELOPE_STAGE_DECAY] = 2.0;
        stageValue[ENVELOPE_STAGE_SUSTAIN] = 1.0;
        stageValue[ENVELOPE_STAGE_RELEASE] = 1.0;
    };

    static void staticCallbackFunction(void *p, EnvelopeStage i, TrigMode m)
    {
        // Get back into the class by treating p as the "this" pointer.
        ((EnvelopeGenerator *)p)->enterStage(i, m);
    }

private:
    EnvelopeStage currentStage;
    TrigMode currentMode;
    TrigOutType currentTrigOut;
    bool trigOutActive;
    double currentStageProcess;
    double currentLevel;
    double currentExpLevel;
    double currentInvLevel;
    uint8_t currentLedIntensity;
    double multiplier;
    double increment;
    double currentShape;
    double sampleRate;
    double stageValue[kNumEnvelopeStages];
    void calculateMultiplier(double startLevel, double endLevel, unsigned long long lengthInSamples);
    unsigned long long currentSampleIndex;
    unsigned long long nextStageSampleIndex;
    void calcLed();
};

#endif
