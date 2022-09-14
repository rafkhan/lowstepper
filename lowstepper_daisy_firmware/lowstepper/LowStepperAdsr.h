#ifndef LOWSTEPPER_ADSR_H 
#define LOWSTEPPER_ADSR_H

#include "daisy_seed.h"
#include "daisysp.h"

#include "util.h"

using namespace daisy;
using namespace daisysp;

struct LowStepperAdsrInput {
  bool isGateHigh;
  float attack;
  float decay;
  float sustain;
  float release;
};

class LowStepperAdsr {
  private:
    float sampleRate;
    float sampleLength;
    Adsr envelope;
  public:
    LowStepperAdsr(float sampleRate);
    LowStepperOutput tick(LowStepperAdsrInput);

    static float mapAttack(float input);
    static float mapDecay(float input);
    static float mapSustain(float input);
    static float mapRelease(float input);
};

#endif