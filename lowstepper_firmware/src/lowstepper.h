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

UI* getUI() {
  return &ui;
}

int MODE_COUNT = 2;
int modeIndex = 0;

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

SteppedLfoTrig t1 = SteppedLfoTrig{timeProvider, ui.eocA, ui.cvOutA};
SteppedLfoTrig t2 = SteppedLfoTrig{timeProvider, ui.eocA, ui.cvOutA};
Mode<BaseMode *> mode1 = Mode<BaseMode *>{&t1, &t2};

SteppedLfoGate g1 = SteppedLfoGate{timeProvider, ui.eocB, ui.cvOutB};
SteppedLfoGate g2 = SteppedLfoGate{timeProvider, ui.eocB, ui.cvOutB};
Mode<BaseMode *> mode2 = Mode<BaseMode *>{&g1, &g2};

Mode<BaseMode *> modeList[] = {mode1, mode2};

// Used to track state in between hardware ticks
class LfoState {
  public:
    double bpm = 0;
    double calculateBpm(bool);
};

double LfoState::calculateBpm(bool clockHigh)
{
  // if (clockHigh)
  // {
  //   uint32_t delta = (micros() - this->lastBpmMicros);
  //   double newBpm = (60000.0 / (delta / 1000)) / 4;
  //   this->lastBpmMicros = micros();
  //   return newBpm;
  // }

  return this->bpm;
}



  // if(BPM_ENABLED) {
  //   lfoStateA.bpm = lfoStateA.calculateBpm(ui.clockInA->checkTrigHigh());
  //   int potRateA = map(ui.potInRateA->getValue(), 1, 1023, 1, 5);
  //   lfoFreqA = lfoStateA.bpm / (15 * pow(2, potRateA));
  // } else {
  //   lfoFreqA = map((double) ui.potInRateA->getValue(), 1, 1023, MIN_LFO_FREQ, MAX_LFO_FREQ);
  // }

  // int chunksA = map(ui.potInSegmentDivideA->getValue(), 1, 1023, 1, 8);
  // double morphA = map((double) ui.potInMorphA->getValue(), 1, 1023, 0, 1);

