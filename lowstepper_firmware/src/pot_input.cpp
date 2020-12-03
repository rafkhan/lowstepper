#include <Arduino.h>

#include "pot_input.h"

PotInput::PotInput(int pinNum) {
  this->pin = pinNum;
}

PotInput::~PotInput() {}

void PotInput::init() {
  pinMode(this->pin, INPUT);
}

void PotInput::scan() {
  this->value = (double) analogRead(this->pin);
}