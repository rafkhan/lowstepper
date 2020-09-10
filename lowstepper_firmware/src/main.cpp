#include <Arduino.h>
#include "lfo.h"

// elapsedMicros usec = 0;

// elapsedMicros usec2 = 0;
// bool up = true;

// copied from O_C
static constexpr uint32_t OC_CORE_ISR_FREQ = 16666U;
static constexpr uint32_t OC_CORE_TIMER_RATE = (1000000UL / OC_CORE_ISR_FREQ);

// Main timer stuff
IntervalTimer main_timer;
volatile uint32_t lastMicros;
volatile uint32_t ticks = 0;

// LFO parameters
volatile double phase = 0;
volatile double morph = 0;
volatile uint32_t lfoFreq = 10;

void FASTRUN main_timer_ISR() {
  uint32_t deltaTimeMicro = micros() - lastMicros;
  double microsPerLfo = 1000000.0 / lfoFreq;
  double ratioThroughLfo = deltaTimeMicro / microsPerLfo;
  phase += ratioThroughLfo * TWO_PI;

  if (phase >= TWO_PI) {
    phase = 0;
  }

  float val = (fuckYeahLfo(morph, phase) * 2000.0) + 2050.0;
  analogWrite(A22, (int)val);
  analogWrite(13, (int)val);
  lastMicros = micros();
}

void setup() {
  analogWriteResolution(12);
  pinMode(13, OUTPUT);

  // Setup timers
  main_timer.begin(main_timer_ISR, OC_CORE_TIMER_RATE);
}

double phaseCopy = 0;

void loop() {
  noInterrupts();
  phaseCopy = phase;
  interrupts();

  Serial.println(phaseCopy);
  // float val = (fuckYeahLfo(morph, phase) * 2000.0) + 2050.0;

  // analogWrite(A22, (int)val);
  // analogWrite(13, (int)val);

  // phase = phase + 0.02;

  // if (phase >= TWO_PI) {
  //   phase = 0;
  // }

  // while (usec < 500) {
  //   // wait
  //   // TODO stabilize timing
  // }

  // if(usec2 > 100) {
  //   if (up) {
  //     morph += 0.0001;
  //   } else {
  //     morph -= 0.0001;
  //   }

  //   if (!(morph < 1 && morph > 0)) {
  //     Serial.println("swap");
  //     up = !up;
  //   }

  //   Serial.println(morph);
  //   usec2 = 0;
  // }

  // usec = usec - 500;
}
