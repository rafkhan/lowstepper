#ifndef STEPPED_LFO_H
#define STEPPED_LFO_H

#include <cmath>
#include <stdio.h>

#include "./LfoFunctions.h"
#include "./BaseMode.h"

class SteppedLfoTrig : public BaseMode
{
public:
  SteppedLfoTrig(TimeProvider *tp,
                 TrigWriter *tw,
                 DACWriter *dw);
  virtual float tick(
      double frequency,
      double morph,
      int divisions,
      bool trigHigh,
      double inputPhase,
      uint32_t lastTickTime);

  volatile bool lfoRunning = false;
  volatile double nextStopPosition = 1;
  volatile float lastWriteValue = 0;
  void incrementNextStep(int divisions);
};

SteppedLfoTrig::SteppedLfoTrig(
    TimeProvider *tp,
    TrigWriter *tw,
    DACWriter *dw)
{
  this->timeProvider = tp;
  this->trigWriter = tw;
  this->dacWriter = dw;
  lastMicros = this->timeProvider->getTime();
}

float SteppedLfoTrig::tick(
    double frequency,
    double morph,
    int divisions,
    bool trigHigh,
    double inputPhase, // used for smooth multimode switching
    uint32_t lastTickTime)
{
  this->phase = inputPhase;
  this->lastMicros = lastTickTime;

  // Figure out if LFO should be running
  if (trigHigh)
  {
    // LFO is already running, skip to next segment
    if (lfoRunning)
    {
      incrementNextStep(divisions);
    }

    this->phase = (TWO_PI / divisions) * (nextStopPosition - 1.0);
    lfoRunning = true;
  }

  if (lfoRunning)
  {
    uint32_t deltaTimeMicro = this->timeProvider->getTime() - lastMicros;
    double inc = (deltaTimeMicro / (1000000.0 / frequency)) * TWO_PI;
    this->phase += inc;

    if (this->phase >= ((TWO_PI / divisions) * nextStopPosition))
    {
      incrementNextStep(divisions);

      // Stop if in Trig mode
      lfoRunning = false;
    }

    if (this->phase >= TWO_PI)
    {
      // Disable this logic when free running
      this->phase = TWO_PI;
    }

    lastWriteValue = getMorphedOutput(morph, phase);

    this->dacWriter->write((int)(lastWriteValue * 2000.0) + 2050.0);
  }

  this->lastMicros = this->timeProvider->getTime();
  return lastWriteValue;
}

void SteppedLfoTrig::incrementNextStep(int divisions)
{
  if (this->nextStopPosition >= divisions)
  {
    this->nextStopPosition = 1.0;
  }
  else
  {
    this->nextStopPosition += 1.0;
  }
}

#endif