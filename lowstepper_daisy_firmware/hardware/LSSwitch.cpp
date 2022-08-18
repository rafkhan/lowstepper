#include "LSSwitch.h"

#include "daisy_seed.h"
#include "daisysp.h"

using namespace daisy;
using namespace daisysp;

void LSSwitch::init(DaisySeed *hw, int pinGpio) {
	gpio.pin  = hw->GetPin(pinGpio);
	gpio.mode = DSY_GPIO_MODE_INPUT;
	gpio.pull = DSY_GPIO_PULLUP;
	dsy_gpio_init(&gpio);

	this->hw = hw;
}

void LSSwitch::readHardware(void) {
	this->gpioValue = dsy_gpio_read(&(this->gpio));
}

bool LSSwitch::isOn() {
	return this->gpioValue;
}