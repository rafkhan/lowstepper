#ifndef STEPPED_LFO_H
#define STEPPED_LFO_H

#include <cmath>
#include <stdio.h>

#include "./LfoFunctions.h"
#include "./Mode.h"

class SteppedLfoTrig : public Mode
{
public:
  SteppedLfoTrig(TrigWriter tw);
  float tick(
    double frequency,
    double morph,
    int divisions,
    bool trigHigh,
    double inputPhase 
  );

  virtual void writeToDAC(int value);
  virtual uint32_t getTime(void);
  void incrementNextStep(int divisions);

  // Internal state
  volatile bool lfoRunning = false;
  volatile double nextStopPosition = 1;
  volatile uint32_t lastMicros = 0;    // used for lfo calcs
  volatile double phase = 0;
  volatile float lastWriteValue = 0;
  TrigWriter trigWriter;
};

SteppedLfoTrig::SteppedLfoTrig(
  // TODO provide here instead in subclasses:
  // DAC writer
  // Time Provider
  TrigWriter tw
)
{
  lastMicros = this->getTime();
  this->trigWriter = tw;
}

float SteppedLfoTrig::tick(
  double frequency,
  double morph,
  int divisions,
  bool trigHigh,
  double inputPhase // used for smooth multimode switching
) {
  this->phase = inputPhase;

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
    uint32_t deltaTimeMicro = this->getTime() - lastMicros;
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

    this->writeToDAC((int) (lastWriteValue * 2000.0) + 2050.0);
  }

  lastMicros = this->getTime();
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

uint32_t SteppedLfoTrig::getTime() {
  // THIS SHOULD NEVER BE CALLED
  return 1;
}

void SteppedLfoTrig::writeToDAC(int value) { /* do nothing :) */ }

#endif