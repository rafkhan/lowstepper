#include <stdio.h>
#include <emscripten/emscripten.h>

#include "src/modes/SteppedLfo.h"


#define __EMSCRIPTEN__ 1

int main(int argc, char ** argv) {
  // tickLFO(0);
  printf("Hello World\n");
  printf("Hello World2\n");
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

class SteppedLfoEmscripten : public SteppedLfo {
public:
  void writeToDAC(int value);
  uint32_t getTime(void);
};

uint32_t SteppedLfoEmscripten::getTime() {
  return globalTime;
}

void SteppedLfoEmscripten::writeToDAC(int value) { /* do nothing :) */ } 

class SteppedLfoEmpscriptenMode {
  public:
    SteppedLfoEmscripten lfoA;
    SteppedLfoEmscripten lfoB;
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
    true,
    trigHighA
  );
  trigHighA = false;
}

#ifdef __cplusplus
}
#endif

