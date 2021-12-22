#include "LowStepper.h"
#include "LowStepperChannel.h"

LowStepper::LowStepper(LowStepperChannel *a, LowStepperChannel *b) {
  this->lsA = a;
  this->lsB = b;
}

LowStepperOutput* LowStepper::tick() {
  LowStepperOutput outA = this->lsA->tick(1, this->lastPhaseA, true);
  LowStepperOutput outB = this->lsB->tick(2, this->lastPhaseB, true);
  this->lastPhaseA = outA.phase;
  this->lastPhaseB = outA.phase;

  LowStepperOutput outs [2]  = {outA, outB};
  return outs;
}