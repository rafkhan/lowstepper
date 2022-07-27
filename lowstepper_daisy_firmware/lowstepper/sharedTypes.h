#ifndef SHAREDTYPES_H 
#define SHAREDTYPES_H

struct LowStepperOutput {
  bool eocGateHigh;
  float cvOutput;
  float phase;
};

struct LowStepperInput {
  float phase;
  float frequency; // hz
  float morph; // 0 - 1
  float start; // 0 - 1

  // NOTE THIS IS NOW "END" AND NOT "LENGTH"
  float length; // 0 - 1 
};

#endif