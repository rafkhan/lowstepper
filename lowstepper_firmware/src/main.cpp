#include <Arduino.h>
#include "lfo.h"
#include "gate_in.h"

elapsedMicros usec2 = 0;
bool up = true;

// copied from O_C
// TODO check these values
static constexpr uint32_t CORE_ISR_FREQ = 10000U; // how much faster can I make this? lmao
static constexpr uint32_t CORE_TIMER_RATE = (1000000UL / CORE_ISR_FREQ);

// LFO parameters
volatile double morph = 0;
volatile double lfoFreq = 75;

volatile double phase = 0;
volatile int lfoRunning = 0;

// Main timer stuff
IntervalTimer main_timer;
volatile uint32_t lastMicros;
volatile uint32_t ticks = 0;

#define PIN_GATE_OUT 25

volatile double nextStopPosition = 1;
volatile double divisons = 2;

GateIn gateIn(A20);

void FASTRUN main_timer_ISR() {
  if(gateIn.checkGateHigh()) {
    if(lfoRunning) {
      if(nextStopPosition == divisons) {
        nextStopPosition = 1.0;
      } else {
        nextStopPosition += 1.0;
      }
    }
    
    phase = (TWO_PI/divisons) * (nextStopPosition - 1.0);

    lfoRunning = 1;
  }

  if(lfoRunning) {
    uint32_t deltaTimeMicro = micros() - lastMicros;
    double inc = (deltaTimeMicro/(1000000.0/lfoFreq)) * TWO_PI;
    phase += inc;

    if (phase >= ((TWO_PI/divisons) * nextStopPosition)) {
      if(nextStopPosition == divisons) {
        nextStopPosition = 1.0;
      } else {
        nextStopPosition += 1.0;
      }

      lfoRunning = 0;
    }

    if (phase >= TWO_PI) {
      phase = 0;
    }

    float val = (sin(phase) * 2000.0) + 2050.0;
    analogWrite(A22, (int)val);
    analogWrite(13, (int)val);

  }

  lastMicros = micros();
}

void setup() {
  analogWriteResolution(12);
  pinMode(13, OUTPUT);
  pinMode(A22, OUTPUT);
  gateIn.init();

  main_timer.begin(main_timer_ISR, CORE_TIMER_RATE);
}

void loop() {
  noInterrupts()
  gateIn.scan();
  interrupts()
}
