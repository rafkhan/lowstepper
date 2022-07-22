#ifndef SHAREDTYPES_H 
#define SHAREDTYPES_H

struct LowStepperOutput {
  bool eocGateHigh;
  float cvOutput;
  float phase; // used for next calculation
};

struct LowStepperInput {
  float phase;
  float frequency;
  float start; // 0 - 1
  float length; // 0 - 1
};

#endif