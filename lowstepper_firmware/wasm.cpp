
#include "emscripten.h"
#include "src/SteppedLfo.h"


#include <stdio.h>
#include <emscripten/emscripten.h>

int main(int argc, char ** argv) {
    printf("Hello World\n");
}

#ifdef __cplusplus
extern "C" {
#endif

EMSCRIPTEN_KEEPALIVE void myFunction(int argc, char ** argv) {
    printf("MyFunction Called\n");
}


EMSCRIPTEN_KEEPALIVE
double sawWave() {
  return saw(0.0)
}


#ifdef __cplusplus
}
#endif

