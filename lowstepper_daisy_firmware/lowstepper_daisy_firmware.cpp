#include <cstddef>
#include <cmath>
#include <limits.h>

#include "daisy_seed.h"
#include "daisysp.h"

#include "lowstepper/LowStepper.h"
#include "lowstepper/LowStepperChannel.h"
#include "lowstepper/AverageBuffer.h"
#include "lowstepper/util.h"

#include "hardware/GateInput.h"

#define DEBUG 1

// PIN MAPPING
#define MUX1ADC 28
#define MUX1S0 27
#define MUX1S1 26
#define MUX1S2 25

#define MUX2ADC 24
#define MUX2S0 3
#define MUX2S1 2
#define MUX2S2 1

#define PIN_SYNC_A 12	
#define PIN_SYNC_DETECT_A 9
#define PIN_RESET_A 13	
#define PIN_RESET_DETECT_A 8

#define PIN_EOC_A 20
#define PIN_EOC_B 19

using namespace daisy;
using namespace daisysp;

// hardware objects
static DaisySeed hw;
GateInput syncA;
GateInput resetA;

// Config
float sampleRate;

const size_t channelCount = 2;
LowStepperOutput outputs[channelCount] = { NULL, NULL };
LowStepperOutput o { 0, 0, false };
LowStepperOutput o2 { 0, 0, false };
LowStepper *lowStepper;

int adcCycleCounter = 0;
float potInputs[8];
float cvInputs[8];

// Output values
float cvCh1 = 0;
float cvCh2 = 0;

// TODO extract BPM code
float bpmA = 0;
int samplesSinceLastSyncTickA = 1;
AverageBuffer<float> bpmAverageA{(size_t) 10, 0};
bool useSyncA;

// Metro for development
Metro tick;
bool metroValue = false;

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

void initGpioIn() {
	syncA.init(&hw, PIN_SYNC_A, PIN_SYNC_DETECT_A);
	resetA.init(&hw, PIN_RESET_A, PIN_RESET_DETECT_A);
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
	// uint16_t y1 = mapFFII(cvCh1 * -1.0, -1.0, 1.0, 0, 4095);
	// uint16_t y2 =	mapFFII(cvCh2 * -1.0, -1.0, 1.0, 0, 4095);

	uint16_t y1 = mapFFII(cvCh1 * -1.0, -1.0, 1.0, 5, 4090);
	uint16_t y2 =	mapFFII(cvCh2 * -1.0, -1.0, 1.0, 5, 4090);

	// accidentally reversed??
	hw.dac.WriteValue(DacHandle::Channel::TWO, y1);
	hw.dac.WriteValue(DacHandle::Channel::ONE, y2);
}

void readGpioIn() {
	syncA.readHardware();
	resetA.readHardware();
}


void AudioCallback(AudioHandle::InterleavingInputBuffer in,
									 AudioHandle::InterleavingOutputBuffer out,
									 size_t size) {
	if(tick.Process()) {
		metroValue = true;
	}

	readAdc();
	readGpioIn();

	for (size_t n = 0; n < size; n += 2) {
		if(syncA.triggerCheck()) {
			bpmA = 60.0f / ((samplesSinceLastSyncTickA + 1.0f) * (1.0f / sampleRate)) / 4;
			bpmAverageA.addValue(bpmA);
			samplesSinceLastSyncTickA = 0;
			useSyncA = true;
		} else {
			samplesSinceLastSyncTickA++;
			samplesSinceLastSyncTickA = samplesSinceLastSyncTickA % (INT_MAX -  1); // todo check this

			if(samplesSinceLastSyncTickA > 48000) {
				useSyncA = false;
			}
		}

		float avgBpmAValue = bpmAverageA.getAverageValue();
		// useSyncA = syncA.isCablePluggedIn() && (avgBpmAValue > 10); // TODO THIS NEVER GOES FALSE

		LowStepperInput inputA;
		inputA.phase = outputs[0].phase;
		inputA.frequency = LowStepper::mapRateInputToFrequency(getRateAInput(), useSyncA, avgBpmAValue);
		inputA.morph = LowStepper::mapMorphInput(getMorphAInput());
		inputA.start = LowStepper::mapStartInput(getStartAInput(), useSyncA);
		inputA.end = LowStepper::mapLengthInput(getLengthAInput(), useSyncA);
		inputA.shouldReset = resetA.triggerCheck();

		LowStepperInput inputB;
		inputB.phase = outputs[1].phase;
		inputB.frequency = LowStepper::mapRateInputToFrequency(getRateBInput(), false, 0);
		inputB.morph = LowStepper::mapMorphInput(getMorphBInput());
		inputB.start = LowStepper::mapStartInput(getStartBInput(), false);
		inputB.end = LowStepper::mapLengthInput(getLengthBInput(), false);
		inputB.shouldReset = false;
 
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

#if DEBUG
  hw.StartLog();
	// Logger<LOGGER_SEMIHOST>::StartLog(true);
	System::Delay(500);
#endif

	initDac();
	initAdc();
	initGpioOut();
	initGpioIn();

	// gate_output.pin  = hw.GetPin(PIN_EOC_A);
	// gate_output.mode = DSY_GPIO_MODE_OUTPUT_PP;
	// gate_output.pull = DSY_GPIO_PULLDOWN;
	// dsy_gpio_init(&gate_output);

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

			Logger<LOGGER_SEMIHOST>::PrintLine(
				"%d, %d, %f, %f",
				syncA.isCablePluggedIn(),
				useSyncA,
				LowStepper::mapStartInput(getStartAInput(), useSyncA),
				LowStepper::mapLengthInput(getLengthAInput(), useSyncA)
			);
		}
#endif
	}

}
