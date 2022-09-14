#include <cstddef>
#include <cmath>
#include <limits.h>

#include "daisy_seed.h"
#include "daisysp.h"

#include "lowstepper/LowStepperLfo.h"
#include "lowstepper/LowStepperAdsr.h"
#include "lowstepper/AverageBuffer.h"
#include "lowstepper/SyncManager.h"
#include "lowstepper/util.h"

#include "hardware/GateInput.h"
#include "hardware/LSSwitch.h"

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

#define PIN_SYNC_B 10
#define PIN_SYNC_DETECT_B 7
#define PIN_RESET_B 14
#define PIN_RESET_DETECT_B 6

#define PIN_SWITCH_A_1 18
#define PIN_SWITCH_A_2 17
#define PIN_SWITCH_B_1 16
#define PIN_SWITCH_B_2 15

#define PIN_EOC_A 20
#define PIN_EOC_B 19

using namespace daisy;
using namespace daisysp;

// hardware objects
static DaisySeed hw;
GateInput syncA;
GateInput resetA;
GateInput syncB;
GateInput resetB;
LSSwitch switchA;
LSSwitch switchB;

// Config
float sampleRate;

// Internal objects
const size_t channelCount = 2;
// LowStepperOutput outputs[channelCount] = { NULL, NULL };
LowStepperOutput lastOutputA { 0, 0, false };
LowStepperOutput lastOutputB { 0, 0, false };
LowStepperLfo *lfoA;
LowStepperLfo *lfoB;
LowStepperAdsr *adsrA;

SyncManager syncManagerA;
SyncManager syncManagerB;

// ADC Input stuff
int adcCycleCounter = 0;
float potInputs[8];
float cvInputs[8];

// Output values
float cvCh1 = 0;
float cvCh2 = 0;

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
	syncB.init(&hw, PIN_SYNC_B, PIN_SYNC_DETECT_B);
	resetB.init(&hw, PIN_RESET_B, PIN_RESET_DETECT_B);

	switchA.init(&hw, PIN_SWITCH_A_1);
	switchB.init(&hw, PIN_SWITCH_B_1);
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

void initSync(float sampleRate) {
	syncManagerA.init(sampleRate);
	syncManagerB.init(sampleRate);
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
	syncB.readHardware();
	resetB.readHardware();

	switchA.readHardware();
	switchB.readHardware();
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
		if(!lfoA || !lfoB || !adsrA) {
			break;
		}

		// TODO refactor this to not be duplicated...
		bool useSyncA = syncManagerA.tick(syncA.triggerCheck());
		float avgBpmValueA = syncManagerA.getBpm();
		bool useFastModeA = !switchA.isOn();

		bool useSyncB = syncManagerB.tick(syncB.triggerCheck());
		float avgBpmValueB = syncManagerB.getBpm();
		bool useFastModeB = !switchB.isOn();

		LowStepperInput inputA;
		inputA.phase = lastOutputA.phase;
		inputA.frequency = LowStepperLfo::mapRateInputToFrequency(getRateAInput(), useSyncA, useFastModeA, avgBpmValueA);
		inputA.morph = LowStepperLfo::mapMorphInput(getMorphAInput());
		inputA.start = LowStepperLfo::mapStartInput(getStartAInput(), useSyncA);
		inputA.end = LowStepperLfo::mapLengthInput(getLengthAInput(), useSyncA);
		inputA.shouldReset = resetA.triggerCheck();

		LowStepperInput inputB;
		inputB.phase = lastOutputB.phase;
		inputB.frequency = LowStepperLfo::mapRateInputToFrequency(getRateBInput(), useSyncB, useFastModeB, avgBpmValueB);
		inputB.morph = LowStepperLfo::mapMorphInput(getMorphBInput());
		inputB.start = LowStepperLfo::mapStartInput(getStartBInput(), useSyncB);
		inputB.end = LowStepperLfo::mapLengthInput(getLengthBInput(), useSyncB);
		inputB.shouldReset = resetB.triggerCheck();
 
		lastOutputA = lfoA->tick(inputA);
		lastOutputB = lfoB->tick(inputB);

		LowStepperAdsrInput adsrInputA = {
			resetA.isGateHigh(),
			LowStepperAdsr::mapAttack(getRateAInput()),
			LowStepperAdsr::mapDecay(getMorphAInput()),
			LowStepperAdsr::mapSustain(getStartAInput()),
			LowStepperAdsr::mapRelease(getLengthAInput())
		};
		LowStepperOutput o = adsrA->tick(adsrInputA);

		// Copy data out of application to send to DAC
		// cvCh1 = lastOutputA.cvOutput;
		cvCh1 = o.cvOutput;
		// cvCh1 = -0.2f;
		cvCh2 = lastOutputB.cvOutput;

		// Don't do anything on audio output
		out[n] = 0;
		out[n + 1] = 0;
	}
}

int main(void) {
	// outputs[0] = o;
	// outputs[1] = o2;

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
	initSync(sampleRate);

	// gate_output.pin  = hw.GetPin(PIN_EOC_A);
	// gate_output.mode = DSY_GPIO_MODE_OUTPUT_PP;
	// gate_output.pull = DSY_GPIO_PULLDOWN;
	// dsy_gpio_init(&gate_output);

	lfoA = new LowStepperLfo(sampleRate);
	lfoB = new LowStepperLfo(sampleRate);
	adsrA = new LowStepperAdsr(sampleRate);

	hw.SetAudioBlockSize(1);
	hw.StartAudio(AudioCallback);
 
	for (;;)
	{
		writeDac();

#if DEBUG
		if(metroValue) {
			metroValue = false;

			// Logger<LOGGER_SEMIHOST>::PrintLine(
			// 	"%d, %d, %f, %f",
			// 	syncA.isCablePluggedIn(),
			// 	useSyncA,
			// 	LowStepper::mapStartInput(getStartAInput(), useSyncA),
			// 	LowStepper::mapLengthInput(getLengthAInput(), useSyncA)
			// );

			// hw.PrintLine(
			// 	"%d, %d, %d, %d",
			// 	syncB.isGateHigh(),
			// 	syncB.isCablePluggedIn(),
			// 	resetB.isGateHigh(),
			// 	resetB.isCablePluggedIn()
			// );

			// hw.PrintLine(
			// 	"%d, %d",
			// 	switchA.isOn(),
			// 	switchB.isOn()
			// );

			// hw.PrintLine(
			// 	"%f",
			// 	LowStepperChannel::mapStartInput(getStartAInput(), useSyncA)
			// );
		}
#endif
	}

}
