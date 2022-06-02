#ifndef SYSTEMTIMER_H
#define SYSTEMTIMER_H
#include <Arduino.h>

hw_timer_t *interfaceTimer = NULL;
uint8_t interfaceTimerFlag = false; //this flag signales the interface read routine to update

hw_timer_t *audioTimer = NULL;
uint8_t audioTimerFlag = false; //this flag signales the audio routine to update

void initAudioTimer();
void initInterfaceTimer();
//interface timer
void IRAM_ATTR onInterfaceTimer();
uint8_t getInterfaceTimer();
void setInterfaceTimer();
//audio timer
void IRAM_ATTR onAudioTimer();
uint8_t getAudioTimer();
void setAudioTimer();

void initAudioTimer()
{
    //audio polling timer
    audioTimer = timerBegin(1, 80, true); // 1us timer
    timerAttachInterrupt(audioTimer, &onAudioTimer, true);
    timerAlarmWrite(audioTimer, 22, true); // 45.45kHz Alarm
    timerAlarmEnable(audioTimer);
}
void initInterfaceTimer()
{
    //interface polling timer
    interfaceTimer = timerBegin(0, 800, true); // 10us timer
    timerAttachInterrupt(interfaceTimer, &onInterfaceTimer, true);
    timerAlarmWrite(interfaceTimer, 1000, true); // 1kHz Alarm
    timerAlarmEnable(interfaceTimer);
}

void IRAM_ATTR onInterfaceTimer()
{
    interfaceTimerFlag = true;
}

uint8_t getInterfaceTimer()
{
    return interfaceTimerFlag;
}

void setInterfaceTimer(uint8_t state)
{
    interfaceTimerFlag = state;
}

void IRAM_ATTR onAudioTimer()
{
    audioTimerFlag = true;
}

uint8_t getAudioTimer()
{
    return audioTimerFlag;
}

void setAudioTimer(uint8_t state)
{
    audioTimerFlag = state;
}

#endif // !SYSTEMTIMER_H