#ifndef LOWSTEPPER_ADSR_H 
#define LOWSTEPPER_ADSR_H

#include "sharedTypes.h"

class LowStepperAdsr {
  private:
    float sampleRate;
    float sampleLength;
  public:
    LowStepperAdsr(float sampleRate);
    LowStepperOutput tick(LowStepperInput);
};

#endif