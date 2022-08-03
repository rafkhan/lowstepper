#ifndef BASEMODE_H
#define BASEMODE_H

#include <stdint.h>
#include "sharedTypes.h"

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