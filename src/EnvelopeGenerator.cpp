#include "EnvelopeGenerator.h"

void EnvelopeGenerator::setTrigOutType(TrigOutType type)
{
    currentTrigOut = type;
}

uint16_t EnvelopeGenerator::nextTrigOut(uint8_t trigTime)
{
    uint16_t trigOut = 0;
    long long remainingSamples = nextStageSampleIndex - currentSampleIndex;
    if (trigOutActive != true)
    {
        trigOutActive = true;
        if (currentStage != ENVELOPE_STAGE_OFF || currentStage != ENVELOPE_STAGE_SUSTAIN)
        {
            if (remainingSamples < 10)
            {
                switch (currentTrigOut)
                {
                case TRIG_ATTACK:
                    if (currentStage == ENVELOPE_STAGE_ATTACK)
                    {
                    }
                    break;

                default:
                    break;
                }
            }
        }
    }
    return trigOut;
}

double EnvelopeGenerator::nextSample()
{
    if (currentStage != ENVELOPE_STAGE_OFF &&
        currentStage != ENVELOPE_STAGE_SUSTAIN)
    {
        if (currentSampleIndex == nextStageSampleIndex)
        {
            EnvelopeStage newStage = static_cast<EnvelopeStage>(
                (currentStage + 1) % kNumEnvelopeStages);
            enterStage(newStage, currentMode);
        }
        if (currentStage == ENVELOPE_STAGE_ATTACK)
        {
            currentExpLevel *= multiplier;
            currentLevel = 1 - currentExpLevel;
        }
        else
        {
            currentExpLevel *= multiplier;
            currentLevel = currentExpLevel;
        }
        // How far the generator is into the current stage:
        currentStageProcess = (currentSampleIndex + 0.0) / nextStageSampleIndex;
        
        calcLed();
        currentSampleIndex++;
    }
    return currentLevel;
}

uint16_t EnvelopeGenerator::nextSample16()
{

    return static_cast<uint16_t>(65535 - (this->nextSample() * 65535));
}



void EnvelopeGenerator::calculateMultiplier(double startLevel, double endLevel, unsigned long long lengthInSamples)
{
    multiplier = 1.0 + ((log(endLevel) - log(startLevel)) / (lengthInSamples));
}

void EnvelopeGenerator::calcLed()
{
    uint8_t ledIntensity;
    if (currentStage == ENVELOPE_STAGE_SUSTAIN)
    {
        ledIntensity = static_cast<uint8_t>(currentLevel * 255);
    }
    else
    {
        ledIntensity = static_cast<uint8_t>(currentStageProcess * 255);
    }

    if (ledIntensity != currentLedIntensity)
    {
        uint8_t channel;
        currentLedIntensity = ledIntensity;
        if (currentStage != ENVELOPE_STAGE_OFF)
        {
            switch (currentStage)
            {
            case ENVELOPE_STAGE_ATTACK:
                channel = 1;
                break;

            case ENVELOPE_STAGE_DECAY:
                channel = 2;
                break;

            case ENVELOPE_STAGE_SUSTAIN:
                channel = 3;
                break;

            case ENVELOPE_STAGE_RELEASE:
                channel = 4;
                break;
            default:
                channel = 0;
                break;
            }
            ledcWrite(channel, ledIntensity);
        }
    }
}
void EnvelopeGenerator::enterStage(EnvelopeStage newStage, TrigMode newMode)
{
    currentStage = newStage;
    currentMode = newMode;
    currentSampleIndex = 0;
    increment = 0.0;
    if (currentStage == ENVELOPE_STAGE_OFF ||
        currentStage == ENVELOPE_STAGE_SUSTAIN)
    {
        nextStageSampleIndex = 0;
    }
    else
    {
        nextStageSampleIndex = stageValue[currentStage] * sampleRate;
    }

    switch (newStage)
    {
    case ENVELOPE_STAGE_OFF:
        ledcAllOff();

        currentLevel = 0.0;
        currentExpLevel = 0.0;
        multiplier = 1.0;
        break;

    case ENVELOPE_STAGE_ATTACK:
        ledcAllOtherOff(LED_ATTACK);

        currentLevel = minimumLevel;
        currentExpLevel = 1.0;
        currentInvLevel = 1.0;
        calculateMultiplier(1.0, currentLevel, nextStageSampleIndex);
        break;

    case ENVELOPE_STAGE_DECAY:
        ledcAllOtherOff(LED_DECAY);
        currentLevel = 1.0;

        currentExpLevel = 1.0;

        calculateMultiplier(currentLevel, fmax(stageValue[ENVELOPE_STAGE_SUSTAIN], minimumLevel), nextStageSampleIndex);
        break;

    case ENVELOPE_STAGE_SUSTAIN:
        if (currentMode == TRIGGER)
        {
            enterStage(ENVELOPE_STAGE_RELEASE, TRIGGER);
        }
        ledcAllOtherOff(LED_SUSTAIN);
        currentLevel = stageValue[ENVELOPE_STAGE_SUSTAIN];
        currentExpLevel = stageValue[ENVELOPE_STAGE_SUSTAIN];

        multiplier = 1.0;
        increment = 0.0;
        break;

    case ENVELOPE_STAGE_RELEASE:
        ledcAllOtherOff(LED_RELEASE);
        // We could go from ATTACK/DECAY to RELEASE,
        // so we're not changing currentLevel here.
        calculateMultiplier(currentLevel, minimumLevel, nextStageSampleIndex);
        break;

    default:
        break;
    }
}

void EnvelopeGenerator::setSampleRate(double newSampleRate)
{
    sampleRate = newSampleRate;
}



void EnvelopeGenerator::setStageValue(EnvelopeStage stage,
                                      double value)
{
    stageValue[stage] = value;
    if (stage == currentStage)
    {
        // Re-calculate the multiplier and nextStageSampleIndex
        if (currentStage == ENVELOPE_STAGE_DECAY ||
            currentStage == ENVELOPE_STAGE_RELEASE)
        {
            double nextLevelValue = currentLevel;
            switch (currentStage)
            {
            case ENVELOPE_STAGE_ATTACK:
                nextLevelValue = currentLevel;
                break;
            case ENVELOPE_STAGE_DECAY:
                nextLevelValue = fmax(stageValue[ENVELOPE_STAGE_SUSTAIN], minimumLevel);
                break;
            case ENVELOPE_STAGE_RELEASE:
                nextLevelValue = minimumLevel;
                break;
            default:
                break;
            }

            // How much of the current stage is left:
            currentStageProcess = (currentSampleIndex + 0.0) / nextStageSampleIndex;
            double remainingStageProcess = 1.0 - currentStageProcess;
            unsigned long long samplesUntilNextStage = remainingStageProcess * value * sampleRate;
            nextStageSampleIndex = currentSampleIndex + samplesUntilNextStage;
            calculateMultiplier(currentLevel, nextLevelValue, samplesUntilNextStage);
        }
        else if (currentStage == ENVELOPE_STAGE_ATTACK)
        {
            double nextLevelValue = currentLevel;

            // How much of the current stage is left:
            currentStageProcess = (currentSampleIndex + 0.0) / nextStageSampleIndex;
            double remainingStageProcess = 1.0 - currentStageProcess;
            unsigned long long samplesUntilNextStage = remainingStageProcess * value * sampleRate;
            nextStageSampleIndex = currentSampleIndex + samplesUntilNextStage;
            calculateMultiplier(1.0, nextLevelValue, samplesUntilNextStage);
        }
        else if (currentStage == ENVELOPE_STAGE_SUSTAIN)
        {
            currentLevel = value;
        }
        if (currentStage == ENVELOPE_STAGE_DECAY &&
            stage == ENVELOPE_STAGE_SUSTAIN && currentStage == ENVELOPE_STAGE_ATTACK)
        {
            // We have to decay to a different sustain value than before.
            // Re-calculate multiplier:
            unsigned long long samplesUntilNextStage = nextStageSampleIndex - currentSampleIndex;
            calculateMultiplier(currentLevel, fmax(stageValue[ENVELOPE_STAGE_SUSTAIN], minimumLevel), samplesUntilNextStage);
        }
    }
}