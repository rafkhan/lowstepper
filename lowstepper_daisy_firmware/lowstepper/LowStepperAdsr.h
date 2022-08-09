#ifndef LOWSTEPPER_ADSR_H 
#define LOWSTEPPER_ADSR_H

#include "sharedTypes.h"

struct LowStepperAdsrInput {}
struct LowStepperAdsrOutput {}


class LowStepperAdsr {
  private:
    float sampleRate;
    float sampleLength;
  public:
    LowStepperAdsr(float sampleRate);
    LowStepperAdsrOutput tick(LowStepperAdsrInput);
};

#endif