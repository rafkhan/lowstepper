#include "LowStepper.h"
#include "LowStepperChannel.h"

#include "./util.h"

LowStepper::LowStepper(LowStepperChannel **channels, size_t channelCount) {
  this->channels = channels;
  this->channelCount = channelCount;
}

void LowStepper::tick(LowStepperInput *inputs, LowStepperOutput outputs[]) {
  for(int c = 0; c < (int) this->channelCount; c++) {
    outputs[c] = this->channels[c]->tick(inputs[c]);
  }
}

float LowStepper::mapRateInputToFrequency(float input) {
  return mapFFFF(input, 0, 1, 0.001, 10);
}
