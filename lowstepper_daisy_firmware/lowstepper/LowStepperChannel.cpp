#include <stdint.h>
#include <math.h>
#include "LowStepperChannel.h"
#include "util.h"
#include "config.h"

float sampleLength = (1/48000)*1000000;

LowStepperChannel::LowStepperChannel() {}

LowStepperOutput LowStepperChannel::tick(
  float frequency,
  float inputPhase,
  bool trigIn
) {
    float phaseIncrement = (sampleLength / (1000000.0 / frequency)) * TWO_PI;
    float phase = inputPhase + phaseIncrement;
    if (phase > TWO_PI)
    {
      phase = phase - TWO_PI;
    }

    LowStepperOutput output;
    output.cvOutput = sin(phase);
    output.eocGateHigh = phase > 0;
    output.phase = phase;
    return output;
}