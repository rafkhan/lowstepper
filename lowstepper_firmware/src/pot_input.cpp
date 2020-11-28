#include <Arduino.h>

#include "pot_input.h"

PotInput::PotInput(int pinNum) {
  this->pin = pinNum;
}

PotInput::~PotInput() {}

void PotInput::init() {
  pinMode(this->pin, INPUT);
}

void PotInput::scan(void) {
  this->value = (double) analogRead(this->pin);
}

double PotInput::getValue(void) {
  return this->value;
}