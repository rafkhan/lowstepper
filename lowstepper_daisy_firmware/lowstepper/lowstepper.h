#include "LowStepperChannel.h"
#include <cstddef>

typedef void (* LSCallback) (LowStepperOutput[], size_t);

class LowStepper {
  public:
    LowStepper(LowStepperChannel **channels, size_t channelCount);
    void tick(LowStepperInput *inputs, LowStepperOutput outputs[]);

    // TODO move these to LFO specific code
    static float mapRateInputToFrequency(float input, bool enableSync, float bpm); // Convert 0-1 value to frequency
    static float mapMorphInput(float input);
    static float mapStartInput(float input, bool enableSync);
    static float mapLengthInput(float length, bool enableSync);

  private:
    LowStepperChannel **channels;
    size_t channelCount;
};
