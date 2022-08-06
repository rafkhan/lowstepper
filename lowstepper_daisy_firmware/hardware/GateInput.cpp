#include "GateInput.h"

#include "daisy_seed.h"
#include "daisysp.h"

using namespace daisy;
using namespace daisysp;

void GateInput::init(DaisySeed *hw, int pinGpio, int pinDetect) {
	gpio.pin  = hw->GetPin(pinGpio);
	gpio.mode = DSY_GPIO_MODE_INPUT;
	gpio.pull = DSY_GPIO_PULLUP;
	dsy_gpio_init(&gpio);

	gpioDetect.pin  = hw->GetPin(pinDetect);
	gpioDetect.mode = DSY_GPIO_MODE_INPUT;
	gpioDetect.pull = DSY_GPIO_PULLUP;
	dsy_gpio_init(&gpioDetect);

	this->hw = hw;
}

void GateInput::readHardware(void) {
	this->gpioValue = dsy_gpio_read(&(this->gpio));
	this->gpioDetectValue = dsy_gpio_read(&(this->gpioDetect));

	if(!this->isGateHigh()) {
		if(!this->lastValueHigh && !this->trigger) {
			this->trigger = true;
		}
		this->lastValueHigh = true;
	} else {
		this->lastValueHigh = false;	
	}
}

bool GateInput::isGateHigh(void) {
	return !this->gpioValue;
}

bool GateInput::isCablePluggedIn(void) {
	return this->gpioDetectValue;
}

bool GateInput::triggerCheck(void) {
	if(this->trigger) {
		this->trigger = false;
		return true;
	}
	
	return false;
}
