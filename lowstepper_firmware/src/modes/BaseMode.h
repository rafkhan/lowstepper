#ifndef BASEMODE_H
#define BASEMODE_H

#include <cmath>

class TimeProvider
{
public:
  virtual uint32_t getTime(void) = 0;
};

class TrigWriter
{
public:
  virtual void setHighForDuration(uint32_t currentTime, uint32_t duration) = 0;
  virtual void sendTrig(uint32_t currentTime) = 0;
  virtual void tick(uint32_t currentTime) = 0;
};

class DACWriter
{
public:
  virtual void write(uint32_t value) = 0;
};

class BaseMode
{
public:
  volatile uint32_t lastMicros = 0;
  volatile double phase = 0;

  TimeProvider* timeProvider;
  TrigWriter* trigWriter;
  DACWriter* dacWriter;

  virtual float tick(
      double frequency,
      double morph,
      int divisions,
      bool trigHigh,
      double inputPhase,
      uint32_t lastTickTime
    ) = 0;
};

#endif