#ifndef STEPPED_LFO_H
#define STEPPED_LFO_H

#include <cmath>
#include <stdio.h>


#include "../util.h"

#define PI 3.14159265359
#define TWO_PI 6.28318530718

/**
 * All of the following functions take a phase input from 0 - TWO_PI
 * and return a value from -1 to 1.
 * 
 * std::sin is also leveraged but not included because sine waves are
 * incredibly based and just do that by default
 * 
 * lmao math
 */
double triangle(double phase)
{
  //REALLY DUMB CODE, NEED TO BE OPTIMIZED!

  if (phase <= PI * 0.5) //FIRST QUARTER
  {
    return 2 + ( -1 * (((PI - phase * 2) + PI) / PI) ); //GO DOWN!
  }

  if(phase >= PI * 1.5) //FOURTH QUARTER
  {
    return -1 + ( -1 * (((TWO_PI - phase * 2) + PI) / PI) ) ; //GO DOWN TOO!
  } 
  
  //SECOND AND THIRD QUARTER 
  return 1 + ( -1 * ((phase*2 - PI) / PI)); //GO UP!

}

double ramp(double phase)
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

double map(double x, double in_min, double in_max, double out_min, double out_max) {
  return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
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
    return sin(phase) + (newMorph * (ramp(phase) - sin(phase)));
  } else {
    newMorph = map(morphPosition, 0.51, 1, 0, 1);
    return ramp(phase) + (newMorph * (triangle(phase) - ramp(phase)));
  }
}

class SteppedLfo
{
public:
  SteppedLfo();
  float tick(
    double frequency,
    double morph,
    int divisions,
    bool shouldUseTrigger,
    bool trigHigh
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
};

SteppedLfo::SteppedLfo(
  // DAC writer
  // Trig writer
  // Time Provider
)
{
  lastMicros = this->getTime();
}

float SteppedLfo::tick(
  double frequency,
  double morph,
  int divisions,

  // trig
  bool shouldUseTrigger,
  bool trigHigh
) {
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

    // Check if segment is complete, stop LFO if so
    if (this->phase >= ((TWO_PI / divisions) * nextStopPosition))
    {
      incrementNextStep(divisions);
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

void SteppedLfo::incrementNextStep(int divisions)
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

uint32_t SteppedLfo::getTime() {
  printf("this should never be called!!!!!!\n");
  return 1;
}

void SteppedLfo::writeToDAC(int value) { /* do nothing :) */ }

#endif