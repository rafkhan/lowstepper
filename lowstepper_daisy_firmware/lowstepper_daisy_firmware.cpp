#include "daisy_seed.h"
#include "daisysp.h"

using namespace daisy;
using namespace daisysp;

#ifndef TWO_PI
#define TWO_PI 6.28318530718
#endif

float sample_rate;

DaisySeed seed;
static Oscillator osc_sine;

class LFO {
	public:
		uint32_t sample = 0;
		double freq = 1;
		float tick(void);
};

float LFO::tick(void) {
	float y = sin(TWO_PI * sample / (sample_rate / freq));
	sample += 1;
	return y;
}

LFO lfo;
float z;

void AudioCallback(AudioHandle::InputBuffer in, AudioHandle::OutputBuffer out, size_t size)
{
	for (size_t i = 0; i < size; i++)
	{
		float y = lfo.tick();
		// float y = osc_sine.Process();
		// float y = -1;

		if(y > 0) {
			z = 1.0f;
		} else {
			z = -1.0f;
		}

		out[0][i] = y;
		out[1][i] = z;
	}
}

int main(void)
{
	seed.Configure();
	seed.Init();
	sample_rate = seed.AudioSampleRate();
	seed.StartAudio(AudioCallback);

	osc_sine.Init(sample_rate);
	osc_sine.SetWaveform(Oscillator::WAVE_SIN);
	osc_sine.SetFreq(1);
	osc_sine.SetAmp(1);

	while(1) {}
}
