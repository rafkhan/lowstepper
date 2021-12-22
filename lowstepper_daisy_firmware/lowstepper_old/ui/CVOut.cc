#include "CVOut.h"

CVOut::CVOut(int pin) {
  this->pin = pin;
}

void CVOut::init() {
  // pinMode(this->pin, OUTPUT);
}

void CVOut::write(uint32_t value) {
  // analogWrite(this->pin, value);
}