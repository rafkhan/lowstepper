#ifndef STEPPED_LFO_GATE_H
#define STEPPED_LFO_GATE_H

#include <stdint.h>
#include <stdio.h>

#include "./LfoFunctions.h"
#include "./BaseMode.h"

class SteppedLfoGate : public BaseMode
{
public:
  SteppedLfoGate(TimeProvider *tp,
                 TrigWriter *tw,
                 DACWriter *dw);
  virtual float tick(
      double frequency,
      double morph,
      int divisions,
      bool trigHigh,
      double inputPhase,
      uint32_t lastTickTime);

  // Internal state
  volatile bool lfoRunning = false;
  volatile double nextStopPosition = 1;
  volatile float lastWriteValue = 0;
};

SteppedLfoGate::SteppedLfoGate(
  TimeProvider *tp,
  TrigWriter *tw,
  DACWriter *dw
)
{
  this->timeProvider = tp;
  this->trigWriter = tw;
  this->dacWriter = dw;
  lastMicros = this->timeProvider->getTime();
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

  // if (trigHigh)
  // {
  //   lfoRunning = true;
  // } else {
  //   lfoRunning = false;
  // }

  if (trigHigh)
  {
    uint32_t deltaTimeMicro = this->timeProvider->getTime() - this->lastMicros;
    double inc = (deltaTimeMicro / (1000000.0 / frequency)) * TWO_PI;

    this->phase += inc;
    if (this->phase > TWO_PI)
    {
      this->phase = this->phase - TWO_PI;
    }

    this->lastWriteValue = getMorphedOutput(morph, phase);

    this->dacWriter->write((int)(lastWriteValue * 2000.0) + 2050.0);
  }

  lastMicros = this->timeProvider->getTime();
  return this->lastWriteValue;
}

#endif