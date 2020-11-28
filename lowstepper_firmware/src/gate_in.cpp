#include <Arduino.h>

#include "gate_in.h"

/**
 * NOTE!!! the circuit forces the pin low when the incoming gate goes high.
 */
GateIn::GateIn(int pinNum) {
  this->pin = pinNum;
  this->isPinLow = false;
  this->isGateHigh = false;
}

GateIn::~GateIn() {}

void GateIn::init() {
  pinMode(this->pin, INPUT);
}

void GateIn::scan(void) {
  int value = digitalReadFast(this->pin);

  // If the pin goes low, but isn't already low, toggle gate on.
  if(!value && !this->isPinLow) {
    this->isPinLow = true;
    this->isGateHigh = true;
    return;
  }

  // pin has gone back high (gate is low) Reset isPinLow so trig can happen again
  if(value) {
    this->isPinLow = false;
    return;
  }
}

// can only be checked once, to avoid infinite retrigger when kept input kept low
bool GateIn::checkGateHigh(void) {
  if(this->isGateHigh) {
    this->isGateHigh = false;
    return true;
  }

  return false;
}