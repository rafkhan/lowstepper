#include "daisy_seed.h"
#include "daisysp.h"

using namespace daisy;
using namespace daisysp;

static DaisySeed seed;
static Oscillator osc;
float sampleRate;

uint16_t map(float x, float in_min, float in_max, int out_min, int out_max)
{
	return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}

float cvCh1 = 0;
float cvCh2 = 0;

void AudioCallback(AudioHandle::InterleavingInputBuffer in,
									 AudioHandle::InterleavingOutputBuffer out,
									 size_t size)

//void AudioCallback(AudioHandle::InputBuffer in, AudioHandle::OutputBuffer out, size_t size)
{

	for (size_t n = 0; n < size; n += 2)
	// for (size_t i = 0; i < size; i++)
	{
		float y = osc.Process();
		cvCh1 = y;
		cvCh2 = y * -1.0f;
		out[n] = y;
		out[n + 1] = y * -1.0f;

		// out[0][i] = y;
		// out[1][i] = y * -1.0f;
	}
}

int main(void)
{
	seed.Configure();
	seed.Init();
	sampleRate = seed.AudioSampleRate();

	System::Delay(1000);

	// init DAC outputs
	DacHandle::Config cfg;
	cfg.bitdepth = DacHandle::BitDepth::BITS_12;
	cfg.buff_state = DacHandle::BufferState::ENABLED;
	cfg.mode = DacHandle::Mode::POLLING;
	cfg.chn = DacHandle::Channel::BOTH;
	seed.dac.Init(cfg);
	seed.dac.WriteValue(DacHandle::Channel::BOTH, 0);
	seed.dac.WriteValue(DacHandle::Channel::ONE, 0); // CV0
	seed.dac.WriteValue(DacHandle::Channel::TWO, 0); // CV1

	System::Delay(1000);

	osc.Init(sampleRate);
	osc.SetWaveform(osc.WAVE_SIN);
	osc.SetFreq(0.1); // audible freq
	osc.SetAmp(1);

	seed.StartAudio(AudioCallback);

	size_t test_counter = 0;
	for (;;)
	{

		test_counter++;
		//Send the latest envelope values to the CV outs
		uint16_t y1 = map(cvCh1, -1.0, 1.0, 0, 4095);
		uint16_t y2 =	map(cvCh2, -1.0, 1.0, 0, 4095);
		seed.dac.WriteValue(DacHandle::Channel::ONE, test_counter);
		seed.dac.WriteValue(DacHandle::Channel::TWO, test_counter);

		seed.DelayMs(25);
	}
}