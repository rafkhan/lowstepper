#include <stdint.h>
#include <math.h>
#include "LowStepperChannel.h"
#include "util.h"
#include "config.h"

float sampleLength = (1.0f/48000.0f)*1000000.0f;

LowStepperChannel::LowStepperChannel() {}

LowStepperOutput LowStepperChannel::tick(LowStepperInput input) {
    float phaseIncrement = (sampleLength / (1000000.0 / input.frequency)) * TWO_PI;
    float phase = input.phase + phaseIncrement;
    if (phase > TWO_PI)
    {
      phase = phase - TWO_PI;
    }

    LowStepperOutput output;
    output.cvOutput = sin(phase);
    output.eocGateHigh = false;
    output.phase = phase;
    return output;
}