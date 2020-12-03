#include <Arduino.h>
#include <cmath>

#include "lfo.h"
#include "gate_in.h"
#include "pot_input.h"

#define PIN_DAC1 A22
#define PIN_DAC0 A21
#define PIN_TRIG_IN A20
#define PIN_CLOCK_IN A19 // TODO build
#define BLOCK_THIS_PIN_TEMPORARILY A16
#define PIN_RATE_POT A9 // TODO build
#define PIN_SEGMENT_DIVIDE_POT A8 // TODO build

// Internal state
volatile double morph = 0;
volatile double lfoFreq = 30;
volatile bool lfoRunning = false;
volatile double nextStopPosition = 1;
volatile double divisons = 2;
volatile double clockInBpm = 0;

// Timer related stuff
IntervalTimer main_timer;
volatile uint32_t lastBpmMicros; // used to track time between clock inputs
volatile uint32_t lastMicros; // used for lfo calcs
volatile double phase = 0;

// copied from O_C
// TODO check these values
static constexpr uint32_t CORE_ISR_FREQ = 5000U; // how much faster can I make this? lmao
static constexpr uint32_t CORE_TIMER_RATE = (1000000UL / CORE_ISR_FREQ);

// Initialize I/O objects
GateIn gateIn(PIN_TRIG_IN);
GateIn clockIn(PIN_CLOCK_IN); // use the same pin on dev board for now (NEED TO BUILD ANOTHER GATE INPUT)
PotInput potInRate(PIN_RATE_POT);
PotInput potInSegmentDivide(PIN_SEGMENT_DIVIDE_POT);
// TODO abstract DAC output

void incrementNextStep() {
  if(nextStopPosition == divisons) {
    nextStopPosition = 1.0;
  } else {
    nextStopPosition += 1.0;
  }
}

void FASTRUN main_timer_ISR() {
  lfoFreq = clockInBpm/60;

  // Figure out if LFO should be running
  if(gateIn.checkGateHigh()) {
    Serial.println(clockInBpm);
    // LFO is already running, skip to next segment
    if(lfoRunning) {
      incrementNextStep();
    }
    
    phase = (TWO_PI/divisons) * (nextStopPosition - 1.0);
    lfoRunning = true;
  }

  if(lfoRunning) {
    uint32_t deltaTimeMicro = micros() - lastMicros;
    double inc = (deltaTimeMicro/(1000000.0/lfoFreq)) * TWO_PI;
    phase += inc; // maybe make this LAND on 2pi?

    // Check if segment is complete, stop LFO if so
    if (phase >= ((TWO_PI/divisons) * nextStopPosition)) {
      incrementNextStep();
      lfoRunning = false;
    }

    // Reset phase when we can
    // TODO maybe do this more intelligently?
    if (phase >= TWO_PI) {
      phase = 0;
    }

    // TODO morph here
    // TODO abstract all of this shit
    float val = (sin(phase) * 2000.0) + 2050.0;
    analogWrite(PIN_DAC1, (int)val);
    analogWrite(13, (int)val);
  }

  lastMicros = micros();
}

void setup() {
  analogWriteResolution(12);
  pinMode(13, OUTPUT);
  pinMode(PIN_DAC1, OUTPUT); // TODO abstract this
  gateIn.init();
  clockIn.init();
  potInRate.init();
  potInSegmentDivide.init();

  lastMicros = micros();
  lastBpmMicros = micros();
  main_timer.begin(main_timer_ISR, CORE_TIMER_RATE);
}

double roundTenth(double v) {
  int c, r, m;
  m = v*100;
  c = m%10;
  r = m/10;
  if(c>=5)
  r++;
  return (double)r/10;
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
  // potInRate.scan();
  // potInSegmentDivide.scan();;
}

void loop() {
  // Block interrupts when reading hardware I/O
  noInterrupts()
  scanAllInputs();
  clockInBpm = roundTenth(calculateBpm()); // should probably do this outside the interrupt lol
  interrupts() // allow interrupts again
}
