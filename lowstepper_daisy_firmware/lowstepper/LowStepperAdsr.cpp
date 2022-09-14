#include "LowStepperAdsr.h"
#include "util.h"

LowStepperAdsr::LowStepperAdsr(float sampleRate) {
  Adsr env;
  this->envelope = env;
  this->envelope.Init(sampleRate);
}

LowStepperOutput LowStepperAdsr::tick(LowStepperAdsrInput input) {
  this->envelope.SetAttackTime(input.attack);
  this->envelope.SetDecayTime(input.decay);
  this->envelope.SetSustainLevel(input.sustain);
  this->envelope.SetReleaseTime(input.release);

  float envValue = mapFFFF(
    this->envelope.Process(input.isGateHigh),
    0, 1, -0.2, 1
  );

  LowStepperOutput output = { false, envValue, 0 };
  return output;
}

float LowStepperAdsr::mapAttack(float input) {
  return mapFFFF(input, 0, 1, 0, 5);
}

float LowStepperAdsr::mapDecay(float input) {
  return mapFFFF(input, 0, 1, 0, 5);
}

float LowStepperAdsr::mapSustain(float input) {
  return input; // sustain is just 0 -> 1
}

float LowStepperAdsr::mapRelease(float input) {
  return mapFFFF(input, 0, 1, 0, 5);
}