#ifndef STEPPED_LFO_H
#define STEPPED_LFO_H

#include "ui.h"
#include "util.h"
#include "Modes.h"

class SteppedLfo: public LfoMode
{
public:
  SteppedLfo();
  void tick(UI* ui);

private:
  // Internal state
  volatile double morph = 0;
  volatile double lfoFreq = 30;
  volatile bool lfoRunning = false;
  volatile double nextStopPosition = 1;
  volatile double divisons = 2;
  volatile double clockInBpm = 0;
  volatile uint32_t lastBpmMicros = 0; // used to track time between clock inputs
  volatile uint32_t lastMicros = 0; // used for lfo calcs
  volatile double phase = 0;

  void incrementNextStep(void);
  double calculateBpm(UI* ui);
};

SteppedLfo::SteppedLfo() {
  lastMicros = micros();
  lastBpmMicros = micros();
}

void SteppedLfo::tick(UI* ui)
{
  Serial.println(ui->potInRate->getValue());
  Serial.println();
  Serial.println(ui->potInSegmentDivide->getValue());
  Serial.println();
  Serial.println(ui->potInMorph->getValue());
  Serial.println();

  clockInBpm = roundTenth(calculateBpm(ui));
  lfoFreq = clockInBpm/60;

  // Figure out if LFO should be running
  if(ui->trigIn->checkGateHigh()) {
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
      phase = phase - TWO_PI; // lol does this work?
    }

    // TODO morph here
    // TODO abstract all of this shit
    float val = (sin(phase) * 2000.0) + 2050.0;
    analogWrite(PIN_DAC1, (int)val);
  }

  lastMicros = micros();
}

double SteppedLfo::calculateBpm(UI* ui) {
  if(ui->trigIn->checkGateHigh()) {
    uint32_t delta = (micros() - lastBpmMicros);
    double bpm = (60000.0 / (delta / 1000)) / 4;
    lastBpmMicros = micros();
    return bpm;
  }

  return this->clockInBpm;
}

void SteppedLfo::incrementNextStep() {
  if(nextStopPosition == divisons) {
    nextStopPosition = 1.0;
  } else {
    nextStopPosition += 1.0;
  }
}

#endif