#include <stdio.h>
#include <emscripten/emscripten.h>

#include "src/modes/SteppedLfoGate.h"
#include "src/modes/SteppedLfoTrig.h"
#include "src/modes/Mode.h"


#define __EMSCRIPTEN__ 1

int main(int argc, char *argv[]) {
  printf("printing to web console");
}

#ifdef __cplusplus
extern "C" {
#endif


volatile double globalBpm = 128;
volatile uint32_t globalTime = 0;
volatile double rateA = 512;
volatile double morphA = 0;
volatile int chunksA = 1;
volatile bool shouldUseTriggerA = true;
volatile bool trigHighA = false;
volatile bool didSetTrigA = false;
volatile bool shouldUseClockA = false;

//===========================================================
// LFO STUB
//===========================================================
class SteppedLfoGateEmscripten : public SteppedLfoGate {
public:
  SteppedLfoEmscripten(TrigWriter tw) : SteppedLfoGate(tw) {};
  void writeToDAC(int value);
  uint32_t getTime(void);
};

uint32_t SteppedLfoGateEmscripten::getTime() { return globalTime; }
void SteppedLfoGateEmscripten::writeToDAC(int value) { /* do nothing :) */ }

//

class SteppedLfoTrigEmscripten : public SteppedLfoGate {
public:
  SteppedLfoEmscripten(TrigWriter tw) : SteppedLfoGate(tw) {};
  void writeToDAC(int value);
  uint32_t getTime(void);
};

uint32_t SteppedLfoTrigEmscripten::getTime() { return globalTime; }
void SteppedLfoTrigEmscripten::writeToDAC(int value) { /* do nothing :) */ } 

//===========================================================
// TW STUB
//===========================================================
class TrigWriterStub : public TrigWriter
{
  public:
    void setHighForDuration(uint32_t currentTime, uint32_t duration);
    void sendTrig(uint32_t currentTime);
};

void TrigWriterStub::setHighForDuration(uint32_t currentTime, uint32_t duration) {}
void TrigWriterStub::sendTrig(uint32_t currentTime) {}

TrigWriterStub tw;

//===========================================================
// EMSCRIPTEN CLASS
//===========================================================

class SteppedLfoEmpscriptenMode {
  public:
    SteppedLfoGateEmscripten lfoA{tw};
    SteppedLfoTrigEmpscripten lfoA{tw};
};

SteppedLfoEmpscriptenMode slem;

EMSCRIPTEN_KEEPALIVE void setGlobalBpm(double b) {
  globalBpm = b;
}

EMSCRIPTEN_KEEPALIVE void setGlobalTime(uint32_t t) {
  globalTime = t;
}

EMSCRIPTEN_KEEPALIVE void setGateA(bool g) {
  // debounce logic below...
  // has to be low before it can be set high again
  if(!didSetTrigA && g) {
    trigHighA = true;
    didSetTrigA = true;
  }

  if(didSetTrigA && !g) {
    didSetTrigA = false;
    trigHighA = false;
  }
}

EMSCRIPTEN_KEEPALIVE void setRateA(int r) {
  rateA = r;
}

EMSCRIPTEN_KEEPALIVE void setMorphA(double m) {
  morphA = m;
}

EMSCRIPTEN_KEEPALIVE void setChunksA(int c) {
  chunksA = c;
}

EMSCRIPTEN_KEEPALIVE float tickLFO(uint32_t t) {
  globalTime = t;
  return slem.lfoA.tick(
    rateA,
    morphA,
    chunksA,
    trigHighA,
    slem.lfoA.phase,
    slem.lfoA.lastMicros
  );
  trigHighA = false;
}

#ifdef __cplusplus
}
#endif

