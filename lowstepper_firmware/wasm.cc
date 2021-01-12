#include <stdio.h>
#include <emscripten/emscripten.h>

#define __EMSCRIPTEN__ 1

#include "src/ui/ui_emscripten.h"
#include "src/SteppedLfoEmscripten.h"

UI ui;
SteppedLfoEmscripten sle;

int main(int argc, char ** argv) {
    printf("Hello World\n");
}

#ifdef __cplusplus
extern "C" {
#endif

EMSCRIPTEN_KEEPALIVE float tickLFO(int argc, char ** argv) {
  // build UI object
  return sle.tick(&ui);
}

EMSCRIPTEN_KEEPALIVE void setTime(uint32_t time) {
  // build UI object
  sle.setTime(time);
}

#ifdef __cplusplus
}
#endif

