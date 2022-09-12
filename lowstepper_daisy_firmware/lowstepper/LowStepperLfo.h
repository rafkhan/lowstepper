#ifndef LOWSTEPPER_LFO_H
#define LOWSTEPPER_LFO_H

#include <stdint.h>
#include "util.h"

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

class LowStepperLfo
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
    LowStepperLfo(float sampleRate);
    LowStepperOutput tick(LowStepperInput);

    static float mapRateInputToFrequency(float input, bool enableSync, bool enableFastMode, float bpm); // Convert 0-1 value to frequency
    static float mapMorphInput(float input);
    static float mapStartInput(float input, bool enableSync);
    static float mapLengthInput(float length, bool enableSync);
};

#endif