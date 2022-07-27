#include "LowStepperChannel.h"
#include <cstddef>

typedef void (* LSCallback) (LowStepperOutput[], size_t);

class LowStepper {
  public:
    LowStepper(LowStepperChannel **channels, size_t channelCount);
    void tick(LowStepperInput *inputs, LowStepperOutput outputs[]);

    static float mapRateInputToFrequency(float input); // Convert 0-1 value to frequency
    static float mapStartInput(float input);
    static float mapLengthInput(float length);

  private:
    LowStepperChannel **channels;
    size_t channelCount;
};
