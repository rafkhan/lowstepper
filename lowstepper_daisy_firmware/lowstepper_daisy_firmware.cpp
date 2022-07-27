#include <cstddef>
#include <cmath>
#include "daisy_seed.h"
#include "daisysp.h"

#include "lowstepper/LowStepper.h"
#include "lowstepper/LowStepperChannel.h"
#include "lowstepper/util.h"

#define DEBUG 1

using namespace daisy;
using namespace daisysp;

// hardware objects
static DaisySeed hw;

dsy_gpio gpioResetA;
uint8_t gpioResetAValue = 0;

dsy_gpio gpioResetDetectA;
uint8_t gpioResetDetectAValue = 0;

// dsy_gpio gate_output;
Metro tick;
bool metroValue = false;

// Config
float sampleRate;

// Globals lol
float cvCh1 = 0;
float cvCh2 = 0;

// PIN MAPPING
const int MUX1ADC = 28;
const int MUX1S0 = 27;
const int MUX1S1 = 26;
const int MUX1S2 = 25;

const int MUX2ADC = 24;
const int MUX2S0 = 3;
const int MUX2S1 = 2;
const int MUX2S2 = 1;

#define PIN_RESET_A 13	
#define PIN_RESET_DETECT_A 8

#define PIN_EOC_A 20
#define PIN_EOC_B 19

const size_t channelCount = 2;
LowStepperOutput outputs[channelCount] = { NULL, NULL };
LowStepperOutput o { 0, 0, false };
LowStepperOutput o2 { 0, 0, false };
LowStepper *lowStepper;

int adcCycleCounter = 0;
float potInputs[8];
float cvInputs[8];

// TODO: Fix all of these mappings...
float getRateAInput() {
	return combinePotAndCv(potInputs[1], cvInputs[5]);
}

float getMorphAInput() {
	return combinePotAndCv(potInputs[0], cvInputs[7]);
}

float getStartAInput() {
	return combinePotAndCv(potInputs[4], cvInputs[6]);
}

float getLengthAInput() {
	return combinePotAndCv(potInputs[6], cvInputs[0]);
}

float getRateBInput() {
	return combinePotAndCv(potInputs[2], cvInputs[4]);
}

float getMorphBInput() {
	return combinePotAndCv(potInputs[3], cvInputs[1]);
}

float getStartBInput() {
	return combinePotAndCv(potInputs[5], cvInputs[2]);
}

float getLengthBInput() {
	return combinePotAndCv(potInputs[7], cvInputs[3]);
}

void initAdc() {
	AdcChannelConfig adc[2];
	adc[0].InitMux(hw.GetPin(MUX1ADC), 8, hw.GetPin(MUX1S0), hw.GetPin(MUX1S1), hw.GetPin(MUX1S2));
	adc[1].InitMux(hw.GetPin(MUX2ADC), 8, hw.GetPin(MUX2S0), hw.GetPin(MUX2S1), hw.GetPin(MUX2S2));
	hw.adc.Init(adc, 2);
	hw.adc.Start();
}

void initGpioIn() {
	gpioResetA.pin  = hw.GetPin(PIN_RESET_A);
	gpioResetA.mode = DSY_GPIO_MODE_INPUT;
	gpioResetA.pull = DSY_GPIO_PULLUP;
	dsy_gpio_init(&gpioResetA);

	gpioResetDetectA.pin  = hw.GetPin(PIN_RESET_DETECT_A);
	gpioResetDetectA.mode = DSY_GPIO_MODE_INPUT;
	gpioResetDetectA.pull = DSY_GPIO_PULLUP;
	dsy_gpio_init(&gpioResetDetectA);
}

void initGpioOut() {
	// gpioOutEocA.pin  = hw.GetPin(PIN_EOC_A);
	// gpioOutEocA.mode = DSY_GPIO_MODE_OUTPUT_PP;
	// gpioOutEocA.pull = DSY_GPIO_NOPULL;
	// dsy_gpio_init(&gpioOutEocA);

	// gpioOutEocB.pin  = hw.GetPin(PIN_EOC_B);
	// gpioOutEocB.mode = DSY_GPIO_MODE_OUTPUT_PP;
	// gpioOutEocB.pull = DSY_GPIO_NOPULL;
	// dsy_gpio_init(&gpioOutEocB);

	// gpioOutEocA.Init() 
}

void initDac() {
	DacHandle::Config cfg;
	cfg.bitdepth = DacHandle::BitDepth::BITS_12;
	cfg.buff_state = DacHandle::BufferState::ENABLED;
	cfg.mode = DacHandle::Mode::POLLING;
	cfg.chn = DacHandle::Channel::BOTH;
	hw.dac.Init(cfg);
	hw.dac.WriteValue(DacHandle::Channel::BOTH, 0);
	hw.dac.WriteValue(DacHandle::Channel::ONE, 0); // CV0
	hw.dac.WriteValue(DacHandle::Channel::TWO, 0); // CV1
}

void readAdc() {
	adcCycleCounter = (adcCycleCounter + 1) % 8;
	potInputs[adcCycleCounter] = 1.0 - hw.adc.GetMuxFloat(0, adcCycleCounter);
	cvInputs[adcCycleCounter] = 1.0 - hw.adc.GetMuxFloat(1, adcCycleCounter);
}

void writeDac() {
	uint16_t y1 = mapFFII(cvCh1 * -1.0, -1.0, 1.0, 0, 4095);
	uint16_t y2 =	mapFFII(cvCh2 * -1.0, -1.0, 1.0, 0, 4095);

	// accidentally reversed??
	hw.dac.WriteValue(DacHandle::Channel::TWO, y1);
	hw.dac.WriteValue(DacHandle::Channel::ONE, y2);
}

void readGpioIn() {

}

void AudioCallback(AudioHandle::InterleavingInputBuffer in,
									 AudioHandle::InterleavingOutputBuffer out,
									 size_t size) {
	if(tick.Process()) {
		metroValue = true;
	}

	gpioResetAValue = dsy_gpio_read(&gpioResetA);
	gpioResetDetectAValue = dsy_gpio_read(&gpioResetDetectA);
	readAdc();

	for (size_t n = 0; n < size; n += 2)
	{
		LowStepperInput inputA;
		inputA.phase = outputs[0].phase;
		inputA.frequency = LowStepper::mapRateInputToFrequency(getRateAInput());
		inputA.morph = LowStepper::mapMorphInput(getMorphAInput());
		inputA.start = LowStepper::mapStartInput(getStartAInput());
		inputA.length = LowStepper::mapLengthInput(getLengthAInput());

		LowStepperInput inputB;
		inputB.phase = outputs[1].phase;
		inputB.frequency = LowStepper::mapRateInputToFrequency(getRateBInput());
		inputB.morph = LowStepper::mapMorphInput(getMorphBInput());
		inputB.start = LowStepper::mapStartInput(getStartBInput());
		inputB.length = LowStepper::mapLengthInput(getLengthBInput());

		LowStepperInput inputs[2] = { inputA, inputB };

		// Process inputs array, dump into outputs array
		lowStepper->tick(inputs, outputs);

		// Copy data out of application to send to DAC
		cvCh1 = outputs[0].cvOutput;
		cvCh2 = outputs[1].cvOutput;

		// Don't do anything on audio output
		out[n] = 0;
		out[n + 1] = 0;
	}
}

int main(void) {
	outputs[0] = o;
	outputs[1] = o2;

	// Init Daisy
	hw.Configure();
	hw.Init();
	hw.SetAudioSampleRate(SaiHandle::Config::SampleRate::SAI_48KHZ);
	sampleRate = hw.AudioSampleRate();

	tick.Init(2.f, sampleRate);

	// gate_output.pin  = hw.GetPin(PIN_EOC_A);
	// gate_output.mode = DSY_GPIO_MODE_OUTPUT_PP;
	// gate_output.pull = DSY_GPIO_PULLDOWN;
	// dsy_gpio_init(&gate_output);

#if DEBUG
  hw.StartLog();
	System::Delay(500);
#endif

	initDac();
	initAdc();
	initGpioIn();
	initGpioOut();

	// NOTE TO SELF: Do not move from main, application will segfault lol
	LowStepperChannel *channelA = new LowStepperChannel(sampleRate); // don't use this
	LowStepperChannel *channelB = new LowStepperChannel(sampleRate);; // don't use this
	LowStepperChannel *lsChannels[channelCount] = { channelA, channelB }; // use this
	lowStepper = new LowStepper(lsChannels, channelCount);

	hw.SetAudioBlockSize(1);
	hw.StartAudio(AudioCallback);
 
	for (;;)
	{
		writeDac();

#if DEBUG
		if(metroValue) {
			metroValue = false;
			// hw.PrintLine("%f, %f", potInputs[1], cvInputs[7]);
			hw.PrintLine("%f, %f, %f, %f, %f, %f, %f, %f", cvInputs[0], cvInputs[1], cvInputs[2], cvInputs[3], cvInputs[4], cvInputs[5], cvInputs[6], cvInputs[7]);
			hw.PrintLine("%f, %f, %f, %f, %f, %f, %f, %f", potInputs[0], potInputs[1], potInputs[2], potInputs[3], potInputs[4], potInputs[5], potInputs[6], potInputs[7]);
			hw.PrintLine("***");
		}
#endif
	}

}
