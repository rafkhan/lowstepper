#ifndef BASEMODE_H
#define BASEMODE_H

#include <stdint.h>
#include "sharedTypes.h"

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