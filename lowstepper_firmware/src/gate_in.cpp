#include <Arduino.h>

#include "gate_in.h"

/**
 * NOTE!!! the circuit forces the pin low when the incoming gate goes high.
 * ground = input is high
 * V+ = input is low
 */
GateIn::GateIn(int pinNum) {
  this->pin = pinNum;
  this->isPinLow = false;
  this->isTrigHigh = false;
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
    this->isTrigHigh = true;
    return;
  }

  // pin has gone back high (gate is low) Reset isPinLow so trig can happen again
  if(value) {
    this->isPinLow = false;
    return;
  }
}

// Trigs can only be checked once, to avoid infinite retrigger when kept input kept low
bool GateIn::checkTrigHigh(void) {
  if(this->isTrigHigh) {
    this->isTrigHigh = false;
    return true;
  }

  return false;
}

// Gates are high if the pin is low
bool GateIn::checkGateHigh(void) {
  return isPinLow;
}