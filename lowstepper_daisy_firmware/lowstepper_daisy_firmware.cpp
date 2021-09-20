#include "daisy_seed.h"
#include "daisysp.h"

using namespace daisy;
using namespace daisysp;

static DaisySeed seed;
static Oscillator osc;
float sample_rate;

int map(float x, float in_min, float in_max, int out_min, int out_max) {
  return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}

void AudioCallback(AudioHandle::InputBuffer in, AudioHandle::OutputBuffer out, size_t size)
{
	for (size_t i = 0; i < size; i++)
	{
		float y = osc.Process();
		int cvCh1 = map(y, -1.0, 1.0, 0, 4095);
		int cvCh2 =	map(y * -1.0f, -1.0, 1.0, 0, 4095);
		seed.dac.WriteValue(DacHandle::Channel::ONE, cvCh1);
		seed.dac.WriteValue(DacHandle::Channel::TWO, cvCh2);

		out[0][i] = y;
		out[1][i] = y * -1.0f;
	}
}

int main(void)
{
	seed.Configure();
	seed.Init();
	sample_rate = seed.AudioSampleRate();
	osc.Init(sample_rate);

	osc.SetWaveform(osc.WAVE_SIN);
	osc.SetFreq(1);
	osc.SetAmp(1);

	seed.StartAudio(AudioCallback);

	while(1) {}
}
