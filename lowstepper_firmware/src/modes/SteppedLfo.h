#ifndef STEPPED_LFO_H
#define STEPPED_LFO_H

#include <cmath>

#include "../util.h"


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

class SteppedLfo
{
public:
  SteppedLfo();
  float tick(
    double frequency,
    double morph,
    int chunks,
    bool shouldUseTrigger,
    bool trigHigh,
    bool shouldUseClock,
    double bpm
  );

protected:
  volatile double lfoFreq = 30;
  volatile double morph = 0;
  volatile int divisions = 2;
  volatile double clockInBpm = 0;

private:
  void incrementNextStep(void);
  uint32_t getTime(void);
  void writeToDAC(int value);

  // Internal state
  volatile bool lfoRunning = false;
  volatile double nextStopPosition = 1;
  volatile uint32_t lastBpmMicros = 0; // used to track time between clock inputs
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
  lastBpmMicros = this->getTime();
}

float SteppedLfo::tick(
  double frequency,
  double morph,
  int chunks,

  // trig
  bool shouldUseTrigger,
  bool trigHigh,

  // clk
  bool shouldUseClock,
  double bpm
) {

  // this->setStateFromExternalInputs(ui);

  // Figure out if LFO should be running
  if (trigHigh)
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

      // potentially move this down
      // ui.eoc1->setHighForDuration(this->getTime(), 50);
    }

    if (phase >= TWO_PI)
    {
      // Disable this logic when free running
      phase = TWO_PI;
    }

    lastWriteValue = getMorphedOutput(morph, phase);

    // TODO abstract all of this shit
    // analogWrite(PIN_DAC1, (int)writeValue);
    // this->writeToDAC((int) (lastWriteValue * 2000.0) + 2050.0);

    // Maybe turn off trig
    // ui.eoc1->tick(this->getTime());
  }

  lastMicros = this->getTime();
  return lastWriteValue;
}

// double SteppedLfo::calculateBpm(UI ui)
// {
//   if (ui.clockIn->checkTrigHigh())
//   {
//     uint32_t delta = (this->getTime() - lastBpmMicros);
//     double bpm = (60000.0 / (delta / 1000)) / 4;
//     lastBpmMicros = this->getTime();
//     return bpm;
//   }

//   return this->clockInBpm;
// }

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


#endif