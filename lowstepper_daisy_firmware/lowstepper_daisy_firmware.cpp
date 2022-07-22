#include <cstddef>
#include <cstdint>

#include "daisy_seed.h"
#include "daisysp.h"

#include "lowstepper/config.h"
#include "lowstepper/util.h"
#include "lowstepper/pins.h"
#include "lowstepper/sharedTypes.h"
#include "lowstepper/LowStepper.h"
#include "lowstepper/LowStepperChannel.h"
#include "lowstepper/GateInput.h"

using namespace daisy;
using namespace daisysp;

static DaisySeed hw;

// Metro tick;
// int eocAValue = 1;
// int eocBValue = 0;
// GPIO gpioOutEocA;
// GPIO gpioOutEocB;

// Globals lol
float cvCh1 = 0;
float cvCh2 = 0;

GateInput syncA;
GateInput syncB;
GateInput resetA;
GateInput resetB;

float cvInputValues[8];
float potInputValues[8];


const size_t channelCount = 2;
LowStepperOutput outputs[channelCount] = { NULL, NULL };
LowStepperOutput o { 0, 0, false };
LowStepperOutput o2 { 0, 0, false };
LowStepper *lowStepper;

int adcCycleCounter = 0;

void readAdc() {
	potInputValues[adcCycleCounter] = hw.adc.GetMuxFloat(0, adcCycleCounter);
	cvInputValues[adcCycleCounter] = hw.adc.GetMuxFloat(1, adcCycleCounter);
	adcCycleCounter = (adcCycleCounter + 1) % 8;
}

void readGateInputs() {

}

void initAdc() {
	AdcChannelConfig adc[2];
	adc[0].InitMux(hw.GetPin(PIN_MUX1ADC), 8, hw.GetPin(PIN_MUX1S0), hw.GetPin(PIN_MUX1S1), hw.GetPin(PIN_MUX1S2));
	adc[1].InitMux(hw.GetPin(PIN_MUX2ADC), 8, hw.GetPin(PIN_MUX2S0), hw.GetPin(PIN_MUX2S1), hw.GetPin(PIN_MUX2S2));
	hw.adc.Init(adc, 2);
	hw.adc.Start();
}

void initGpioIn() {
	// gpioResetA.pin  = hw.GetPin(PIN_RESET_A);
	// gpioResetA.mode = DSY_GPIO_MODE_INPUT;
	// gpioResetA.pull = DSY_GPIO_PULLUP;
	// dsy_gpio_init(&gpioResetA);

	// gpioResetDetectA.pin  = hw.GetPin(PIN_RESET_DETECT_A);
	// gpioResetDetectA.mode = DSY_GPIO_MODE_INPUT;
	// gpioResetDetectA.pull = DSY_GPIO_PULLUP;
	// dsy_gpio_init(&gpioResetDetectA);

	resetA.init(&hw, PIN_RESET_A, PIN_RESET_DETECT_A);
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
	hw.dac.WriteValue(DacHandle::Channel::ONE, y1);
	hw.dac.WriteValue(DacHandle::Channel::TWO, y2);
}

void AudioCallback(AudioHandle::InterleavingInputBuffer in,
									 AudioHandle::InterleavingOutputBuffer out,
									 size_t size)
{

	// gpioResetAValue = dsy_gpio_read(&gpioResetA);
	// gpioResetDetectAValue = dsy_gpio_read(&gpioResetDetectA);
	readAdc();
	readGateInputs();

	// if(tick.Process()) {
		// dsy_gpio_write(&gpioOutEocA, eocAValue);
		// dsy_gpio_write(&gpioOutEocB, eocBValue);
		// gpioOutEocA.Write(eocAValue);
		// gpioOutEocB.Write(eocBValue);
		// eocAValue	= !eocAValue;
		// eocBValue	= !eocBValue;
		// gpioOutEocA.Toggle();
		// gpioOutEocB.Toggle();
	// }

	for (size_t n = 0; n < size; n += 2) { // why did I make n += 2 here? why two???
		LowStepperInput input;
		input.frequency = 100;
		input.phase = outputs[0].phase;
		input.start = 0.8f;
		input.length = 0.5;

		LowStepperInput input2;
		input2.frequency = 20;
		input2.phase = outputs[1].phase;
		input2.start = 0.1f;
		input2.length = 1;

		LowStepperInput inputs[2] = { input, input2 };

		// Process inputs array, dump into outputs array
		lowStepper->tick(inputs, outputs);

		float y = outputs[0].cvOutput;
		cvCh1 = outputs[0].cvOutput;
		cvCh2 = outputs[1].cvOutput;

		// Don't do anything on audio output
		out[n] = 0;
		out[n + 1] = 0;
	}
}

int main(void) {
	#if LOWSTEPPER_DEBUG
		hw.StartLog();
		System::Delay(500);
	#endif

	// weird code... can I move all of the initialization of outputs array here?
	outputs[0] = o;
	outputs[1] = o2;

	// Init Daisy
	hw.Configure();
	hw.Init();
	hw.SetAudioSampleRate(SaiHandle::Config::SampleRate::SAI_48KHZ);
	float sampleRate = hw.AudioSampleRate();
	// tick.Init(10.f, sampleRate);

	initDac();
	initAdc();
	initGpioIn();
	initGpioOut();

	// NOTE TO SELF: Do not move from main, application will segfault lol
	// Maybe add init method?
	LowStepperChannel *channelA = new LowStepperChannel(sampleRate); // don't use this
	LowStepperChannel *channelB = new LowStepperChannel(sampleRate);; // don't use this
	LowStepperChannel *lsChannels[channelCount] = { channelA, channelB }; // use this
	lowStepper = new LowStepper(lsChannels, channelCount);

	hw.SetAudioBlockSize(1);
	hw.StartAudio(AudioCallback);
 
	for (;;) {
		writeDac();

		#if LOWSTEPPER_DEBUG
				hw.PrintLine(
					"1: %f\t2: %f\t3: %f\t4: %f\t5:%f\t6:%f\t7:%f\t8:%f\t",
					potInputValues[0],
					potInputValues[1],
					potInputValues[2],
					potInputValues[3],
					potInputValues[4],
					potInputValues[5],
					potInputValues[6],
					potInputValues[7]
				);
				// hw.PrintLine("RESET A: %d\tRESET DETECT A:%d", gpioResetAValue, gpioResetDetectAValue);
				// System::Delay(10);
		#endif
	}
}
