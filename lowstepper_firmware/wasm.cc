#include <stdio.h>
#include <emscripten/emscripten.h>

#define __EMSCRIPTEN__ 1

#include "src/ui/ui_emscripten.h"
#include "src/SteppedLfoEmscripten.h"

UI ui;
SteppedLfoEmscripten sle;

int main(int argc, char ** argv) {
  ui.setGate(0);
  printf("Hello World\n");
}

#ifdef __cplusplus
extern "C" {
#endif

EMSCRIPTEN_KEEPALIVE float tickLFO(uint32_t time) {
  // build UI object
  sle.setTime(time);
  ui.scan();
  return sle.tick(&ui);
}

EMSCRIPTEN_KEEPALIVE void setGate(int g) {
  // build UI object
  ui.setGate(g);
}

EMSCRIPTEN_KEEPALIVE uint32_t getTime(void) {
  // build UI object
  return sle.getTime();
}

#ifdef __cplusplus
}
#endif

