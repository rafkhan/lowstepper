#include <stdint.h>
#include <math.h>
#include "LowStepperChannel.h"
#include "util.h"

LowStepperChannel::LowStepperChannel(float sampleRate) {
  this->sampleRate = sampleRate;
  this->sampleLength = (1.0f/sampleRate)*1000000.0f;
}

LowStepperOutput LowStepperChannel::tick(LowStepperInput input) {
    float phaseIncrement = (this->sampleLength / (1000000.0 / input.frequency)) * TWO_PI;
    float phase = input.phase + phaseIncrement;
    float startPhase = TWO_PI * input.start;
    float endPhase = startPhase + (TWO_PI * input.length);
    // float endPhase = TWO_PI * input.length;

    if (phase > endPhase) {
      phase = startPhase + (phase - endPhase);
    }

    LowStepperOutput output;
    output.cvOutput = sin(phase);
    output.eocGateHigh = false;
    output.phase = phase;
    return output;
}
