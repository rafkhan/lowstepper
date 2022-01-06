#ifndef BASEMODE_H
#define BASEMODE_H

#include <stdint.h>

class GateOutput
{
public:
  virtual void outputGateForDuration(uint32_t sampleDuration) = 0;
  virtual void outputTrig(void) = 0;
  virtual void tick(void) = 0;
};

struct LowStepperOutput {
  bool eocGateHigh;
  float cvOutput;
  float phase;
};

struct LowStepperInput {
  float phase;
  float frequency;
  float start; // 0 - 1
  float length; // 0 - 1
};

class LowStepperChannel
{
  private:
    float sampleRate;
    float sampleLength;
  public:
    LowStepperChannel(float sampleRate);
    LowStepperOutput tick(LowStepperInput);
};

#endif