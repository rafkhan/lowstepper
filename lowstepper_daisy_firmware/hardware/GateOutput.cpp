#include "GateOutput.h"

#include "daisy_seed.h"
#include "daisysp.h"

using namespace daisy;
using namespace daisysp;

void GateOutput::init(DaisySeed *hw, float sampleRate, float blockSize, int pinGpio) {
	this->hw = hw;
	this->sampleRate = sampleRate;
	this->blockSize = blockSize;

	this->gpio.pin  = this->hw->GetPin(pinGpio);
	this->gpio.mode = DSY_GPIO_MODE_OUTPUT_PP;
	this->gpio.pull = DSY_GPIO_NOPULL;
	dsy_gpio_init(&this->gpio);

	this->sampleCountEnd = 250;
}

void GateOutput::setGateHigh(int ms) {
	sampleCount = 0;
	dsy_gpio_write(&gpio, true);
}

void GateOutput::tick(void) {
	if (sampleCount > sampleCountEnd) {
		dsy_gpio_write(&gpio, false);
		sampleCount = 0;
	}

	sampleCount += blockSize;
}
