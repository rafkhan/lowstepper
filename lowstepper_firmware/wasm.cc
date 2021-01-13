#include <stdio.h>
#include <emscripten/emscripten.h>

#define __EMSCRIPTEN__ 1

#include "src/ui/ui_emscripten.h"
#include "src/SteppedLfoEmscripten.h"

UI ui;
SteppedLfoEmscripten sle;

int main(int argc, char ** argv) {
  ui.setGate(true);
  // tickLFO(0);
  printf("Hello World\n");
}

#ifdef __cplusplus
extern "C" {
#endif

EMSCRIPTEN_KEEPALIVE float tickLFO(uint32_t time) {
  sle.setTime(time);
  ui.scan();
  return sle.tick(ui);
}

EMSCRIPTEN_KEEPALIVE void setGate(int g) {
  ui.setGate(g);
}

EMSCRIPTEN_KEEPALIVE uint32_t getTime(void) {
  return sle.getTime();
}

EMSCRIPTEN_KEEPALIVE void setRate(double freq) {
  return sle.setLfoFreq(freq);
}

EMSCRIPTEN_KEEPALIVE void setMorph(double morph) {
  return sle.setMorph(morph);
}

EMSCRIPTEN_KEEPALIVE void setChunks(int c) {
  return sle.setChunks(c);
}

#ifdef __cplusplus
}
#endif

