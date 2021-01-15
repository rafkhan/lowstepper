#ifndef STEPPED_LFO_H
#define STEPPED_LFO_H

#include <cmath>

#ifdef __EMSCRIPTEN__
#include <emscripten/emscripten.h>
#include "ui/ui_emscripten.h"
#else
#include "ui/ui.h"
#endif

#include "util.h"
#include "Mode.h"

  // if(potRate == 1) {
  //   Serial.println("1/4");
  // } else if(potRate == 2) {
  //   Serial.println("1/2");
  // } else if(potRate == 3) {
  //   Serial.println("1");
  // } else if(potRate == 4) {
  //   Serial.println("2");
  // } else if(potRate == 5) {
  //   Serial.println("4");
  // }

/**
 * All of the following functions take a phase input from 0 - TWO_PI
 * and return a value from 0 - 1.
 * 
 * std::sin is also leveraged but not included because sine waves are
 * incredibly based and just do that by default
 * 
 * lmao math
 */
double triangle(double phase)
{
  // TODO fix this... wtf did I do after jupyter?
  double unadjustedAmp = std::abs(std::fmod((phase - PI / 2), TWO_PI) - PI);
  return unadjustedAmp * 2 - 1;
}

double saw(double phase)
{
  return -1 * ((phase - PI) / PI);
}

double square(double phase)
{
  if (phase >= PI)
  {
    return -1;
  }
  return 1;
}

/**
 * Note to self: read your damn whiteboard, but write it here anyway
 * 
 * w = value between 0 and 1, weight to morph between functions f and g
 * x = phase between 0 and 2pi
 * f = function that is returned when w = 0
 * g = function that is returned when w = 1
 * 
 * I(w, x, f, g) = f(x) + w(g(x) - f(x))
 * I(x) = morphed wave function
 */
double getMorphedOutput(double morphPosition, double phase)
{
  double newMorph;
  if(morphPosition <= 0.5) {
    newMorph = map(morphPosition, 0, 0.5, 0, 1);
    return sin(phase) + (newMorph * (saw(phase) - sin(phase)));
  } else {
    newMorph = map(morphPosition, 0.51, 1, 0, 1);
    return saw(phase) + (newMorph * (square(phase) - saw(phase)));
  }
}

class SteppedLfo : public Mode
{
public:
  SteppedLfo();
  float tick(UI ui);

protected:
  volatile double lfoFreq = 30;
  volatile double morph = 0;
  volatile int divisions = 2;
  volatile double clockInBpm = 0;
  double calculateBpm(UI ui);

private:
  void incrementNextStep(void);
  uint32_t getTime(void);
  void writeToDAC(int value);
  virtual void setStateFromExternalInputs(UI ui);

  // Internal state
  volatile bool lfoRunning = false;
  volatile double nextStopPosition = 1;
  volatile uint32_t lastBpmMicros = 0; // used to track time between clock inputs
  volatile uint32_t lastMicros = 0;    // used for lfo calcs
  volatile double phase = 0;
  volatile float lastWriteValue = 0;
};

SteppedLfo::SteppedLfo()
{
  lastMicros = this->getTime();
  lastBpmMicros = this->getTime();
}

float SteppedLfo::tick(UI ui)
{
  this->setStateFromExternalInputs(ui);

  // Figure out if LFO should be running
  if (ui.trigIn->checkTrigHigh())
  {
    // Serial.println(ui->potInRate->getValue());
    // LFO is already running, skip to next segment
    if (lfoRunning)
    {
      incrementNextStep();
    }

    phase = (TWO_PI / divisions) * (nextStopPosition - 1.0);
    lfoRunning = true;
  }

  if (lfoRunning)
  {
    uint32_t deltaTimeMicro = this->getTime() - lastMicros;
    double inc = (deltaTimeMicro / (1000000.0 / lfoFreq)) * TWO_PI;
    phase += inc;

    // Check if segment is complete, stop LFO if so
    if (phase >= ((TWO_PI / divisions) * nextStopPosition))
    {
      incrementNextStep();
      lfoRunning = false;
    }

    // Reset phase when we can
    // TODO maybe do this more intelligently?
    if (phase >= TWO_PI)
    {
      phase = TWO_PI; // lol does this work?
    }

    lastWriteValue = getMorphedOutput(morph, phase);

    // TODO abstract all of this shit
    // analogWrite(PIN_DAC1, (int)writeValue);
    this->writeToDAC((int) (lastWriteValue * 2000.0) + 2050.0);
  }

  lastMicros = this->getTime();
  return lastWriteValue;
}

double SteppedLfo::calculateBpm(UI ui)
{
  if (ui.clockIn->checkTrigHigh())
  {
    uint32_t delta = (this->getTime() - lastBpmMicros);
    double bpm = (60000.0 / (delta / 1000)) / 4;
    lastBpmMicros = this->getTime();
    return bpm;
  }

  return this->clockInBpm;
}

void SteppedLfo::incrementNextStep()
{
  if (nextStopPosition >= divisions)
  {
    nextStopPosition = 1.0;
  }
  else
  {
    nextStopPosition += 1.0;
  }
}

uint32_t SteppedLfo::getTime(void)
{
  return 0;
}

void SteppedLfo::writeToDAC(int value)
{
  return;
}

void SteppedLfo::setStateFromExternalInputs(UI ui)
{
  return;
}


#endif