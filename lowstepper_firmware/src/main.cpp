#include <Arduino.h>
#include <cmath>

#include "lfo.h"
#include "gate_in.h"
#include "pot_input.h"

#define DAC1_PIN A22

// copied from O_C
// TODO check these values
static constexpr uint32_t CORE_ISR_FREQ = 5000U; // how much faster can I make this? lmao
static constexpr uint32_t CORE_TIMER_RATE = (1000000UL / CORE_ISR_FREQ);

// LFO parameters
volatile double morph = 0;
volatile double lfoFreq = 30;

volatile double phase = 0;
volatile int lfoRunning = 0;
volatile double nextStopPosition = 1;
volatile double divisons = 2;
volatile double clockInBpm = 0;
volatile uint32_t lastBpmMicros;

// Main timer stuff
IntervalTimer main_timer;
volatile uint32_t lastMicros;
volatile uint32_t ticks = 0;

GateIn gateIn(A20);
GateIn clockIn(A20); // use the same pin on dev board for now (NEED TO BUILD ANOTHER GATE INPUT)

PotInput potIn(A19);

void incrementNextStep() {
  if(nextStopPosition == divisons) {
    nextStopPosition = 1.0;
  } else {
    nextStopPosition += 1.0;
  }
}

void FASTRUN main_timer_ISR() {
  if(gateIn.checkGateHigh()) {
    if(lfoRunning) {
      incrementNextStep();
    }
    
    phase = (TWO_PI/divisons) * (nextStopPosition - 1.0);
    lfoRunning = 1;
  }

  if(lfoRunning) {
    uint32_t deltaTimeMicro = micros() - lastMicros;
    double inc = (deltaTimeMicro/(1000000.0/lfoFreq)) * TWO_PI;
    phase += inc;

    if (phase >= ((TWO_PI/divisons) * nextStopPosition)) {
      incrementNextStep();
      lfoRunning = 0;
    }

    if (phase >= TWO_PI) {
      phase = 0;
    }

    // TODO morph here
    float val = (sin(phase) * 2000.0) + 2050.0;
    analogWrite(DAC1_PIN, (int)val);
    analogWrite(13, (int)val);
  }

  lastMicros = micros();
}

void setup() {
  analogWriteResolution(12);
  pinMode(13, OUTPUT);
  pinMode(DAC1_PIN, OUTPUT);
  gateIn.init();
  clockIn.init();
  // potIn.init();

  lastMicros = micros();
  lastBpmMicros = micros();
  main_timer.begin(main_timer_ISR, CORE_TIMER_RATE);
}

double calculateBpm() {
  if(clockIn.checkGateHigh()) {
    uint32_t delta = (micros() - lastBpmMicros);
    double bpm = (60000.0 / (delta / 1000)) / 4;
    lastBpmMicros = micros();
    return bpm;
  }

  return clockInBpm;
}

void scanAllInputs() {
  gateIn.scan();
  clockIn.scan();
  // potIn.scan();
}

void loop() {
  noInterrupts()
  scanAllInputs();
  clockInBpm = calculateBpm();
  interrupts()
}
