#include <Arduino.h>

#include "TrigOut.h"

TrigOut::TrigOut(int pin) {
  this->pin = pin;
  this->isHigh = true;
}

void TrigOut::init() {
  pinMode(this->pin, OUTPUT);
}

void TrigOut::tick(uint32_t currentTime) {
  if(this->isHigh && currentTime >= this->endTime) {
    setLow();
  }
}

void TrigOut::setHighForDuration(uint32_t currentTime, uint32_t duration) {
  this->startTime = currentTime;
  this->endTime = this->startTime + duration;
  this->setHigh();
}

void TrigOut::setHigh(void) {
  digitalWriteFast(this->pin, HIGH);
  isHigh = true;
}

void TrigOut::setLow(void) {
  digitalWriteFast(this->pin, LOW);
  isHigh = false;
}