#ifndef BASEMODE_H
#define BASEMODE_H

#include <stdint.h>

class TimeProvider
{
public:
  virtual uint32_t getTime(void) = 0;
};

class GateOutput
{
public:
  virtual void setHighForDuration(uint32_t currentTime, uint32_t duration) = 0;
  virtual void sendTrig(uint32_t currentTime) = 0;
  virtual void tick(uint32_t currentTime) = 0;
};

struct LowStepperOutput {
  float dac1;
  float dac2;
  GateOutput eocA;
  GateOutput eocB;
};

class BaseMode
{
  public:
    volatile uint32_t lastMicros = 0;
    volatile double phase = 0;

    TimeProvider* timeProvider;

    virtual float tick(
        uint32_t lastTickTime
    ) = 0;
};

#endif