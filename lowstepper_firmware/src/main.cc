#include <Arduino.h>
#include <cmath>

#include "util.h"
#include "ui/UI.h"
#include "SteppedLfoHardware.h"

// Timer related stuff
IntervalTimer main_timer;

// copied from O_C
// TODO check these values
static constexpr uint32_t CORE_ISR_FREQ = 3000U; // how much faster can I make this? lmao
static constexpr uint32_t CORE_TIMER_RATE = (1000000UL / CORE_ISR_FREQ);

UI ui;
SteppedLfoHardware steppedLfo(PIN_DAC1);

void FASTRUN main_timer_ISR() {
  steppedLfo.tick(ui);
}

void setup() {
  analogWriteResolution(12);
  pinMode(13, OUTPUT);

  ui.init();

  digitalWrite(13, HIGH); // power alive light
  main_timer.begin(main_timer_ISR, CORE_TIMER_RATE);
}

void loop() {
  // Block interrupts when reading hardware I/O
  noInterrupts()
  ui.scan();
  // allow interrupts again
  interrupts()
}
