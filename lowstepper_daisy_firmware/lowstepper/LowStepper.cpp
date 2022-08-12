#include <cmath>

#include "LowStepper.h"
#include "LowStepperChannel.h"
#include "util.h"

LowStepper::LowStepper(LowStepperChannel **channels, size_t channelCount) {
  this->channels = channels;
  this->channelCount = channelCount;
}

void LowStepper::tick(LowStepperInput *inputs, LowStepperOutput outputs[]) {
  for(int c = 0; c < (int) this->channelCount; c++) {
    outputs[c] = this->channels[c]->tick(inputs[c]);
  }
}

float LowStepper::mapRateInputToFrequency(float input, bool enableSync, float bpm) {
  if(enableSync) {
    float maxFreq = (bpm / 60.0f) * 4;
    float exponent = floor(mapFFFF(input, 0, 1, 8, 0)); // will never actually go to 17?
    float position = pow(2, exponent);
    return maxFreq / (float) position;
  } else {
    // TODO: non-linear curve here.
    return mapFFFF(input, 0, 1, 0.05, 2);
  }
}

float LowStepper::mapMorphInput(float input) {
  return input;
}

float LowStepper::mapStartInput(float input, bool enableSync) {
  if(enableSync) {
    int exponent = (int) floor(mapFFFF(input, 0, 1, 0, 4));
    float position = pow(2, exponent);

    return exponent == 0 ? 0 : 1.0f / (float) position;
  }
  return input;
  // return mapFFFF(input, 0, 1, 0, 1); // TODO fix 0.1
}

float LowStepper::mapLengthInput(float input, bool enableSync) {
  // return mapFFFF(input, 0, 1, 0, 1); // TODO fix 0.1
  return input;
}

