#include "LowStepperChannel.h"
#include <cstddef>

typedef void (* LSCallback) (LowStepperOutput[], size_t);

class LowStepper {
  public:
    LowStepper(LowStepperChannel **channels, size_t channelCount);
    void tick(LowStepperInput *inputs, LowStepperOutput outputs[]);

    static float mapRateInputToFrequency(float input); // Conver 0-1 value to frequency

  private:
    LowStepperChannel **channels;
    size_t channelCount;
};
