#include <Arduino.h>

#include "GateOut.h"

GateOut::GateOut(int pin) {
  this->pin = pin;
  this->isHigh = true;
}

void GateOut::init() {
  pinMode(this->pin, OUTPUT);
}

void GateOut::tick(uint32_t currentTime) {
  if(this->isHigh && currentTime >= this->endTime) {
    setLow();
  }
}

void GateOut::setHighForDuration(uint32_t currentTime, uint32_t duration) {
  this->startTime = currentTime;
  this->endTime = this->startTime + duration;
  this->setHigh();
}

void GateOut::sendTrig(uint32_t currentTime) {}

void GateOut::setHigh(void) {
  digitalWriteFast(this->pin, HIGH);
  isHigh = true;
}

void GateOut::setLow(void) {
  digitalWriteFast(this->pin, LOW);
  isHigh = false;
}