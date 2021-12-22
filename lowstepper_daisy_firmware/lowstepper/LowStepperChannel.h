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

class LowStepperChannel
{
  public:
    LowStepperChannel();
    LowStepperOutput tick(
      float frequency,
      float inputPhase,
      bool trigIn
    );
};

#endif