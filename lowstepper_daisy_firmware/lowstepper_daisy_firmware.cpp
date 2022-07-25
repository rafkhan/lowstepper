#include <cstddef>
#include <cmath>
#include "daisy_seed.h"
#include "daisysp.h"

#include "lowstepper/LowStepper.h"
#include "lowstepper/LowStepperChannel.h"
#include "lowstepper/util.h"

#define DEBUG 1

uint16_t mapFFII(float x, float in_min, float in_max, int out_min, int out_max)
{
	return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}

using namespace daisy;
using namespace daisysp;

// hardware objects
static DaisySeed hw;

dsy_gpio gpioResetA;
uint8_t gpioResetAValue = 0;

dsy_gpio gpioResetDetectA;
uint8_t gpioResetDetectAValue = 0;

// dsy_gpio gate_output;
// Metro tick;
// bool metroValue = false;

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

float getRateA() {
	return combinePotAndCv(potInputs[1], cvInputs[7]);
}

float getMorphA() {
	return combinePotAndCv(potInputs[2], cvInputs[5]);
}

float getStartA() {
	return combinePotAndCv(potInputs[4], cvInputs[4]);
}

float getEndA() {
	return combinePotAndCv(potInputs[6], cvInputs[6]);
}

void readAdc() {
	adcCycleCounter = (adcCycleCounter + 1) % 8;
	potInputs[adcCycleCounter] = 1.0 - hw.adc.GetMuxFloat(0, adcCycleCounter);
	cvInputs[adcCycleCounter] = hw.adc.GetMuxFloat(1, adcCycleCounter);
}

void AudioCallback(AudioHandle::InterleavingInputBuffer in,
									 AudioHandle::InterleavingOutputBuffer out,
									 size_t size)
{

	// if(tick.Process()) {
	// 	dsy_gpio_write(&gate_output, metroValue); 
	// 	metroValue = !metroValue;
	// }

	gpioResetAValue = dsy_gpio_read(&gpioResetA);
	gpioResetDetectAValue = dsy_gpio_read(&gpioResetDetectA);
	readAdc();

	for (size_t n = 0; n < size; n += 2)
	{
		LowStepperInput input;
		input.phase = outputs[0].phase;
		input.frequency = LowStepper::mapRateInputToFrequency(getRateA());
		input.start = potInputs[4];
		input.length = potInputs[6];

		LowStepperInput input2;
		input2.phase = outputs[1].phase;
		input2.frequency = 20;
		input2.start = 0.1f;
		input2.length = 1;

		LowStepperInput inputs[2] = { input, input2 };

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

void writeDac(void) {
	uint16_t y1 = mapFFII(cvCh1, -1.0, 1.0, 0, 4095);
	uint16_t y2 =	mapFFII(cvCh2, -1.0, 1.0, 0, 4095);

	// accidentally reversed??
	hw.dac.WriteValue(DacHandle::Channel::TWO, y1);
	hw.dac.WriteValue(DacHandle::Channel::ONE, y2);
}

int main(void) {
	outputs[0] = o;
	outputs[1] = o2;

	// Init Daisy
	hw.Configure();
	hw.Init();
	hw.SetAudioSampleRate(SaiHandle::Config::SampleRate::SAI_48KHZ);
	sampleRate = hw.AudioSampleRate();

	// tick.Init(1.f, sampleRate);
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
		// hw.PrintLine("1: %f\t2: %f\t3: %f\t4: %f\t5:%f\t6:%f\t7:%f\t8:%f\t", mux1In0, mux1In1, mux1In2, mux1In3, mux1In4, mux1In5, mux1In6, mux1In7);
		// hw.PrintLine("1: %f\t2: %f\t3: %f\t4: %f\t5:%f\t6:%f\t7:%f\t8:%f\t", mux2In0, mux2In1, mux2In2, mux2In3, mux2In4, mux2In5, mux2In6, mux2In7);
		// hw.PrintLine("RESET A: %d\tRESET DETECT A:%d", gpioResetAValue, gpioResetDetectAValue);
		// hw.PrintLine("%d / %d", eocAValue, eocBValue);
		hw.PrintLine("%f, %f", potInputs[1], cvInputs[7]);
		// System::Delay(10);
#endif
	}

}
