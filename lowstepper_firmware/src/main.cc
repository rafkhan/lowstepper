#include <Arduino.h>
#include <cmath>

#include "ui/UI.h"
#include "lowstepper.h"

#define DEBUG_POT_VALUES 0
#define DEBUG_CV_IN_VALUES 1
#define DEBUG_TRIGGER_IN 0
#define DEBUG_TRIGGER_OUT 0
#define DEBUG_CV_OUT 0

// Timer related stuff
IntervalTimer main_timer;

static constexpr uint32_t CORE_ISR_FREQ = 3000U; // how much faster can I make this? lmao
static constexpr uint32_t CORE_TIMER_RATE = (1000000UL / CORE_ISR_FREQ);

void FASTRUN main_timer_ISR()
{
  // MAIN CODE HERE
}

void setup()
{
  analogWriteResolution(12);

  // power alive light
  pinMode(13, OUTPUT);
  digitalWrite(13, HIGH);

  getUI()->init();

  main_timer.begin(main_timer_ISR, CORE_TIMER_RATE);
}

#if DEBUG_CV_OUT
float debug_cv_phase = 0.0;
elapsedMicros debug_cv_usec = 0;
#endif

#if DEBUG_TRIGGER_OUT
uint32_t debug_eoc_a_time = millis();
uint32_t debug_eoc_b_time = millis();
#endif

void loop()
{
  UI *ui = getUI();
  // Block interrupts when reading hardware I/O
  noInterrupts()
      ui->scan();
  // allow interrupts again
  interrupts()

/********************************************
 * DEBUG CODE BELOW
 * ENABLE VIA FLAGS AT THE TOP OF THIS FILE 
 ********************************************/

#if DEBUG_POT_VALUES
      Serial.printf(
          "POTS:\tRATE_A: %d \t RATE_B: %d \t MORPH_A: %d \t MORPH_B: %d \t CHUNKS_A: %d \t CHUNKS_B: %d\n",
          ui->potInRateA->getValue(),
          ui->potInRateB->getValue(),
          ui->potInMorphA->getValue(),
          ui->potInMorphB->getValue(),
          ui->potInChunksA->getValue(),
          ui->potInChunksB->getValue());
#endif

#if DEBUG_CV_IN_VALUES
  Serial.printf(
      "CV:\tRATE_A: %d \t RATE_B: %d \t MORPH_A: %d \t MORPH_B: %d \t CHUNKS_A: %d \t CHUNKS_B: %d\n",
      ui->cvInRateA->getValue(),
      ui->cvInRateB->getValue(),
      ui->cvInMorphA->getValue(),
      ui->cvInMorphB->getValue(),
      ui->cvInChunksA->getValue(),
      ui->cvInChunksB->getValue());
#endif

#if DEBUG_TRIGGER_IN
  Serial.printf(
      "TRIG_A: %d/%d,\t CLK_A: %d/%d,\t RST_A: %d/%d\t",
      ui->trigInA->isCablePluggedIn(),
      ui->trigInA->isGateHigh(),
      ui->clockInA->isCablePluggedIn(),
      ui->clockInA->isGateHigh(),
      ui->resetInA->isCablePluggedIn(),
      ui->resetInA->isGateHigh());

  Serial.printf(
      "TRIG_B: %d/%d,\t CLK_B: %d/%d,\t RST_B: %d/%d\n",
      ui->trigInB->isCablePluggedIn(),
      ui->trigInB->isGateHigh(),
      ui->clockInB->isCablePluggedIn(),
      ui->clockInB->isGateHigh(),
      ui->resetInB->isCablePluggedIn(),
      ui->resetInB->isGateHigh());
#endif

#if DEBUG_CV_OUT
  double y = sin(debug_cv_phase);
  float valA = y * 2000.0 + 2050.0;
  float valB = y * -2000.0 + 2050.0;
  ui->cvOutA->write(valA);
  ui->cvOutB->write(valB);
  debug_cv_phase = debug_cv_phase + 0.02;
  if (debug_cv_phase >= TWO_PI)
  {
    debug_cv_phase = 0;
  }
  while (debug_cv_usec < 500)
  {
    // wait
  }
  debug_cv_usec = debug_cv_usec - 500;
#endif

#if DEBUG_TRIGGER_OUT
  if ((millis() - debug_eoc_a_time) > 1000)
  {
    ui->eocA->setHighForDuration(millis(), 500);
    debug_eoc_a_time = millis();
  }

  if ((millis() - debug_eoc_b_time) > 500)
  {
    ui->eocB->setHighForDuration(millis(), 250);
    debug_eoc_b_time = millis();
  }

  ui->eocA->tick(millis());
  ui->eocB->tick(millis());
#endif
}
