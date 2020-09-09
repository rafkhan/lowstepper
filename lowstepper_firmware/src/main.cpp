#include <Arduino.h>
#include "lfo.h"

double phase = 0.0;
elapsedMicros usec = 0;

double morph = 0;
elapsedMicros usec2 = 0;
bool up = true;

void setup() {
  analogWriteResolution(12);
  pinMode(13, OUTPUT);
}

void loop() {
  float val = (fuckYeahLfo(morph, phase) * 2000.0) + 2050.0;

  analogWrite(A22, (int)val);
  analogWrite(13, (int)val);

  phase = phase + 0.02;

  if (phase >= TWO_PI) {
    phase = 0;
  }

  while (usec < 500) {
    // wait
    // TODO stabilize timing
    if (morph < 1 && morph > 0) {
      if (up) {
        morph += 0.0001;
      } else {
        morph -= 0.0001;
      }
    } else {
      up = !up;
    }
  }

  usec = usec - 500;
}