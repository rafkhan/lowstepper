#ifndef BASEMODE_H
#define BASEMODE_H

#include <cmath>

class TrigWriter
{
public:
  void setHighForDuration(uint32_t currentTime, uint32_t duration);
  void sendTrig(uint32_t currentTime);
};

class BaseMode
{
public:
  volatile uint32_t lastMicros = 0;
  volatile double phase = 0;
  virtual float tick(
      double frequency,
      double morph,
      int divisions,
      bool trigHigh,
      TrigWriter *tw,
      double inputPhase,
      uint32_t lastTickTime
    ) = 0;
  virtual uint32_t getTime(void) = 0;
  virtual void writeToDAC(int value) = 0;
};

#endif