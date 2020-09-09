#include <Arduino.h>
#include "lfo.h"

float phase = 0.0;
elapsedMicros usec = 0;

void setup() {
  analogWriteResolution(12);
  pinMode(13, OUTPUT);
}

void loop() {
  float val = (saw(phase) * 2000.0) + 2050.0;
  analogWrite(A22, (int)val);
  analogWrite(13, (int)val);
  phase = phase + 0.02;

  if (phase >= twopi) {
    phase = 0;
  }

  while (usec < 500) {
    // wait
    // TODO stabilize timing
  }
  usec = usec - 500;
}