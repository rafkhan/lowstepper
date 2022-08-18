#include "LowStepperChannel.h"
#include <cstddef>

typedef void (* LSCallback) (LowStepperOutput[], size_t);

class LowStepper {
  public:
    LowStepper(LowStepperChannel **channels, size_t channelCount);
    void tick(LowStepperInput *inputs, LowStepperOutput outputs[]);
  private:
    LowStepperChannel **channels;
    size_t channelCount;
};
