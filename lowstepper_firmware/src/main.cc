#include <Arduino.h>
#include <cmath>

#include "ui/UI.h"
#include "lowstepper.h"

#define DEBUG_POT_VALUES 0
#define DEBUG_CV_IN_VALUES 0
#define DEBUG_TRIGGER_IN 1
#define DEBUG_TRIGGER_OUT 1
#define DEBUG_CV_OUT 1

// Timer related stuff
IntervalTimer main_timer;

// copied from O_C
// TODO check these values
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

void loop()
{
  UI *ui = getUI();
  // Block interrupts when reading hardware I/O
  noInterrupts()
      // allow interrupts again
      ui->scan();
  interrupts()

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
}
