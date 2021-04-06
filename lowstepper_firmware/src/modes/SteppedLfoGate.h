#ifndef STEPPED_LFO_GATE_H
#define STEPPED_LFO_GATE_H

#include <cmath>
#include <stdio.h>

#include "./LfoFunctions.h"
#include "./BaseMode.h"

class SteppedLfoGate : public BaseMode
{
public:
  SteppedLfoGate();
  virtual float tick(
    double frequency,
    double morph,
    int divisions,
    bool trigHigh,
    double inputPhase,
    uint32_t lastTickTime
  );

  virtual void writeToDAC(int value);
  virtual uint32_t getTime(void);

  // Internal state
  volatile bool lfoRunning = false;
  volatile double nextStopPosition = 1;
  volatile float lastWriteValue = 0;
};

SteppedLfoGate::SteppedLfoGate()
{
  lastMicros = this->getTime();
}

float SteppedLfoGate::tick(
  double frequency,
  double morph,
  int divisions,
  bool trigHigh,
  double inputPhase,
  uint32_t lastTickTime
) {
  this->phase = inputPhase;
  this->lastMicros = lastTickTime;

  // Always run high
  if (trigHigh)
  {
    lfoRunning = true;
  }


  if (lfoRunning)
  {
    uint32_t deltaTimeMicro = this->getTime() - lastMicros;
    double inc = (deltaTimeMicro / (1000000.0 / frequency)) * TWO_PI;

    this->phase += inc;
    if(this->phase > TWO_PI) {
      this->phase = this->phase - TWO_PI;
    }

    this->lastWriteValue = getMorphedOutput(morph, phase);

    this->writeToDAC((int) (lastWriteValue * 2000.0) + 2050.0);
  }

  lastMicros = this->getTime();
  return this->lastWriteValue;
}

uint32_t SteppedLfoGate::getTime() {
  // THIS SHOULD NEVER BE CALLED
  return 1;
}

void SteppedLfoGate::writeToDAC(int value) { /* do nothing :) */ }

#endif