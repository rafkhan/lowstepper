#include <stdio.h>
#include <emscripten/emscripten.h>

#define __EMSCRIPTEN__ 1

#include "src/ui/ui_emscripten.h"
#include "src/modes/SteppedLfoEmscripten.h"


int main(int argc, char ** argv) {
  // tickLFO(0);
  printf("Hello World\n");
}

#ifdef __cplusplus
extern "C" {
#endif

double globalBpm = 128;
uint32_t globalTime = 0;
double frequency = 512;
double morphA = 1;
int chunksA = 1;
bool shouldUseTriggerA = true;
bool trigHighA = false;
bool shouldUseClockA = false;

class SteppedLfoEmscripten : SteppedLfo {};

int SteppedLfoEmscripten::getTime() {
  return globalTime;
}

void SteppedLfoEmscripten::writeToDAC(int value) {} 

class SteppedLfoMode {
  public:
    SteppedLfoEmscripten lfoA;
    SteppedLfoEmscripten lfoB;
};


EMSCRIPTEN_KEEPALIVE void setGlobalBpm(double bpm) {
  globalBpm = bpm;
}

EMSCRIPTEN_KEEPALIVE void setGlobalTime(uint32_t time) {
  globalTime = time;
}

EMSCRIPTEN_KEEPALIVE void setGateA(int g) {
}

EMSCRIPTEN_KEEPALIVE void setRate(double freq) {
}

EMSCRIPTEN_KEEPALIVE void setMorph(double morph) {
}

EMSCRIPTEN_KEEPALIVE void setChunks(int c) {
}

EMSCRIPTEN_KEEPALIVE float tickLFO(uint32_t time) {
  // DO STUFF
}

#ifdef __cplusplus
}
#endif

