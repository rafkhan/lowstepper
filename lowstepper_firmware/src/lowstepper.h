#include <cmath>
#include "modes/SteppedLfoGate.h"
#include "modes/SteppedLfoTrig.h"
#include "modes/BaseMode.h"
#include "modes/Mode.h"
#include "ui/ui.h"

#define BPM_ENABLED true
#define MIN_LFO_FREQ 0.01 
#define MAX_LFO_FREQ 20000

UI ui;
UI* ui_ptr = &ui;

UI* getUI() {
  return ui_ptr;
}

/*
 * Real time code for hardware
 */
class LSTimeProvider: public TimeProvider
{
public:
  virtual uint32_t getTime(void);
};

uint32_t LSTimeProvider::getTime()
{
  return micros();
}
LSTimeProvider *timeProvider = new LSTimeProvider();

/*
 * The actual LFOs!
 */
SteppedLfoTrig t1 = SteppedLfoTrig{timeProvider, ui_ptr->eocA, ui_ptr->cvOutA};
SteppedLfoTrig t2 = SteppedLfoTrig{timeProvider, ui_ptr->eocB, ui_ptr->cvOutB};

SteppedLfoGate g1 = SteppedLfoGate{timeProvider, ui_ptr->eocA, ui_ptr->cvOutA};
SteppedLfoGate g2 = SteppedLfoGate{timeProvider, ui_ptr->eocB, ui_ptr->cvOutB};

/*
 * External LFO state
 */
class LowStepperState {
  public:
    uint32_t lastBpmTime = 0;
    double bpm = 0;
    BaseMode* activeMode;

    LowStepperState(BaseMode* mode);
    void tick(
      PotIn* potRatePtr,
      PotIn* potInChunksPtr,
      PotIn* potInMorphPtr,
      GateIn* gateIn
    );
};

LowStepperState::LowStepperState(BaseMode* mode) {
  this->activeMode = mode;
}

void LowStepperState::tick(
  PotIn* potRatePtr,
  PotIn* potInChunksPtr,
  PotIn* potInMorphPtr,
  GateIn* gateIn
) {
  double freq;
  uint32_t tickTime = timeProvider->getTime();
  double potRateA = map((double) potRatePtr->getValue(), 1, 1023, 1, 5);

  if(ui_ptr->clockInA->isCablePluggedIn()) {
    if (ui_ptr->clockInA->checkTrigHigh())
    {
      uint32_t delta = (tickTime - this->lastBpmTime);
      this->bpm = (60000.0 / (delta / 1000)) / 4;
      this->lastBpmTime = tickTime;
    }

    freq = this->bpm / (15 * pow(2.0, potRateA));
    if (this->bpm <= 0.1) {
      //TODO remove duplicate
      freq = map((double) potRatePtr->getValue(), 1, 1023, 0.01, 10);
    }
  } else {
    freq = map((double) potRatePtr->getValue(), 1, 1023, 0.01, 10);
  }

  int chunksAPot = map(potInChunksPtr->getValue(), 1, 1023, 1, 16);
  // int chunksACV = map(ui_ptr->cvInChunksA->getValue(), 1, 1023, 1, 16);

  double morphAPot = map((double) potInMorphPtr->getValue(), 1, 1023, 0, 1);
  // double morphACV = map((double) ui_ptr->cvInMorphA->getValue(), 1, 1023, 0, 1);

  bool isTrigHigh = gateIn->checkTrigHigh();

  this->activeMode->tick(
    freq,
    morphAPot,
    chunksAPot,
    isTrigHigh,
    this->activeMode->phase,
    this->activeMode->lastMicros
  ); 
}

LowStepperState channelA{&t1};
LowStepperState channelB{&t2};

void selectaRunThaRecord(void) {
  uint32_t tickTime = timeProvider->getTime();
  ui_ptr->eocA->tick(tickTime);
  ui_ptr->eocB->tick(tickTime);
  channelA.tick(
    ui_ptr->potInRateA,
    ui_ptr->potInChunksA,
    ui_ptr->potInMorphA,
    ui_ptr->trigInA
  );
  channelB.tick(
    ui_ptr->potInRateB,
    ui_ptr->potInChunksB,
    ui_ptr->potInMorphB,
    ui_ptr->trigInB
  );
}
