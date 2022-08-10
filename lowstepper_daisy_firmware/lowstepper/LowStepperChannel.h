#ifndef BASEMODE_H
#define BASEMODE_H

#include <stdint.h>
#include "sharedTypes.h"

struct LowStepperOutput {
  bool eocGateHigh;
  float cvOutput;
  float phase;
};

// Input for LFO only
// TODO: Rename this struct
struct LowStepperInput {
  float phase;
  float frequency; // hz
  float morph; // 0 - 1
  float start; // 0 - 1

  float end; // 0 - 1 

  bool shouldReset;
};

class LowStepperChannel
{
  private:
    float sampleRate;
    float sampleLength;

    float square(float phase);
    float triangle(float phase);
    float ramp(float phase);
    float saw(float phase);
    float getMorphedOutput(float morphPosition, float phase);
  public:
    LowStepperChannel(float sampleRate);
    LowStepperOutput tick(LowStepperInput);
};

#endif