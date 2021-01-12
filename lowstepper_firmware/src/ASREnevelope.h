#ifndef ASRENVELOPE_H
#define ASRENVELOPE_H

#include <cmath>

#include "ui/ui.h"
#include "util.h"
#include "Mode.h"

#define ASRENV_SEG_ATTACK 0
#define ASRENV_SEG_SUSTAIN 1
#define ASRENV_SEG_RELEASE 2

class ASREnvelope : public Mode
{
public:
  ASREnvelope();
  void tick(UI *ui);

  volatile uint32_t attackTime = 250000; // 0.25s
  volatile uint32_t releaseTime = 250000; // 0.25s

  volatile uint32_t startTime = 0;
  volatile uint32_t releaseStartTime = 0;
  bool isActive = false;
  int mode = 0;
  double position = 0;
};

void ASREnvelope::tick(UI *ui) {
  uint32_t tickTime = micros();
  if(!isActive && ui->trigIn->checkGateHigh()) {
    isActive = true;
    mode = 0;
    startTime = micros();
    position = 0;
  }

  // attack
  if(ui->trigIn->checkGateHigh() && mode == 0) {
    uint32_t dt = tickTime - startTime;
    if(dt <= attackTime) {
      position = ((double) dt) / attackTime;
    } else {
      mode = 1;
    }
  }



  if(ui->trigIn->checkGateHigh() && mode == 1) {
    position = 1.0;
  }

  if(!ui->trigIn->checkGateHigh() && mode == 1) {
    releaseStartTime = tickTime;
    mode = 2;
  }

  if(mode == 2) {
    uint32_t dt = releaseStartTime - tickTime;
    position = 1 - (((double) dt) / releaseTime);

    if(dt > releaseTime) {
      position = 0;
      mode = 1000;
      isActive = false;
    }
  }



  analogWrite(PIN_DAC1, (int) plusMinusOneToAnalogOut(position));
}


#endif