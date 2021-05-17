#include <Arduino.h>

#include "AnalogIn.h"

AnalogIn::AnalogIn(int pinNum) {
  this->pin = pinNum;
  this->value = 0;
}

AnalogIn::~AnalogIn() {}

void AnalogIn::init() {
  pinMode(this->pin, INPUT);
}

void AnalogIn::scan() {
  this->value = 1024 - analogRead(this->pin);
}

int AnalogIn::getValue() {
  return this->value;
}