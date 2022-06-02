#ifndef DEFINES_H
#define DEFINES_H

#include <Arduino.h>
#include <functional>

#define FW_VERS 1
#define SAMPLERATE 45454.545 // Audio sample rate. 44.1khz is standard CD quality sound.

//SPI DAC
#define DAC_SS 5
#define DAC_SCLK 18
#define DAC_DIN 23

//DAC
#define ESP_DAC 25

//ADC
#define ADC_BITS 4095.0
#define POT_ATTACK 27
#define CV_ATTACK 26
#define POT_DECAY 2
#define CV_DECAY 32
#define POT_SUSTAIN 33
#define CV_SUSTAIN 35
#define POT_RELEASE 36
#define CV_RELEASE 34
#define POT_SHAPE 39

//DIGITAL INPUTS
#define MUX 3
#define S0 16
#define S1 17
#define S2 22
#define TRIG_IN 0
#define GATE_IN 1
#define SW_TRIG 2
#define TOG_HIGH 4
#define SW_TYPE 5
#define TOG_LOW 6

//LEDS
#define LED_ATTACK_PIN 0
#define LED_DECAY_PIN 4
#define LED_SUSTAIN_PIN 19
#define LED_RELEASE_PIN 21
#define LED_TRIG_PIN 1

#define LED_TRIG 0
#define LED_ATTACK 1
#define LED_DECAY 2
#define LED_SUSTAIN 3
#define LED_RELEASE 4

#endif // !DEFINES_H