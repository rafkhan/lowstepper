#include "ui/ui.h"
#include "SteppedLfoTrig.h"

#define BPM_ENABLED true
#define MIN_LFO_FREQ 0.01 
#define MAX_LFO_FREQ 20000

// // Used to track state in between hardware ticks
// class LfoState {
//   public:
//     uint32_t lastBpmMicros = 0;
//     double bpm = 0;
//     double calculateBpm(bool);
// };

// double LfoState::calculateBpm(bool clockHigh)
// {
//   if (clockHigh)
//   {
//     uint32_t delta = (micros() - this->lastBpmMicros);
//     double newBpm = (60000.0 / (delta / 1000)) / 4;
//     this->lastBpmMicros = micros();
//     return newBpm;
//   }

//   return this->bpm;
// }

// /**
//  * Deal with hardware for Dual Stepped LFOs
//  */
// class SteppedLfoMode {
//   public:
//     void tick(UI ui);
//   private:
//     LfoState lfoStateA;
//     LfoState lfoStateB;
//     // SteppedLfoHardware lfoA;
//     // SteppedLfoHardware lfoB;
// };

// void SteppedLfoMode::tick(UI ui) {
//   double lfoFreqA = 0;

//   // TODO add DacWriter and Trig out abstractions

//   // CLK Jacked plugged in
//   if(BPM_ENABLED) {
//     lfoStateA.bpm = lfoStateA.calculateBpm(ui.clockInA->checkTrigHigh());
//     int potRateA = map(ui.potInRateA->getValue(), 1, 1023, 1, 5);
//     lfoFreqA = lfoStateA.bpm / (15 * pow(2, potRateA));
//   } else {
//     lfoFreqA = map((double) ui.potInRateA->getValue(), 1, 1023, MIN_LFO_FREQ, MAX_LFO_FREQ);
//   }

//   int chunksA = map(ui.potInSegmentDivideA->getValue(), 1, 1023, 1, 8);
//   double morphA = map((double) ui.potInMorphA->getValue(), 1, 1023, 0, 1);

//   lfoA.tick(
//     lfoFreqA,
//     morphA,
//     chunksA,
//     true,
//     ui.trigInA->checkTrigHigh()
//   );
// }
