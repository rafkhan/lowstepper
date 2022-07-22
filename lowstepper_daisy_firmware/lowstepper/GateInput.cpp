#include "GateInput.h"
#include "daisy_seed.h"

void GateInput::init(daisy::DaisySeed *seed, uint8_t pin, uint8_t pinDetect) {
  this->gpio.pin  = seed->GetPin(pin);
	this->gpio.mode = DSY_GPIO_MODE_INPUT;
	this->gpio.pull = DSY_GPIO_PULLUP;
	dsy_gpio_init(&this->gpio);

  this->gpioDetect.pin  = seed->GetPin(pin);
	this->gpioDetect.mode = DSY_GPIO_MODE_INPUT;
	this->gpioDetect.pull = DSY_GPIO_PULLUP;
	dsy_gpio_init(&this->gpio);
}
