#include "daisy_seed.h"
#include "daisysp.h"
#include "cuteop.h"

#define NUM_OSC 6

using namespace daisy;
using namespace daisysp;

// Declare a DaisySeed object called hardware
DaisySeed hardware;
Oscillator osc[NUM_OSC];
Oscillator lfo[NUM_OSC];

void AudioCallback(float *in, float *out, size_t size)
{
    // Fill the block with samples
    for (size_t i = 0; i < size; i += 2)
    {
        float sig = 0;

        // Get next osc sample
        // NOTE: pot controlling frequency -
        // osc[0].SetFreq(mtof(hardware.adc.GetFloat(0) * 127));
        for (int j = 0; j < NUM_OSC; j++)
        {
            sig += (osc[j].Process() * lfo[j].Process());
        }
        // Set the left and right outputs
        out[i] = sig;
        out[i + 1] = sig;
    }
}

int main(void)
{
    t_banks banks;
    float lfoArray[NUM_OSC] = {0.1, 0.22, 0.8, 1, 0.43, 0.12};

    // Configure and Initialize the Daisy Seed
    // These are separate to allow reconfiguration of any of the internal
    // components before initialization.
    hardware.Configure();
    hardware.Init();

    // How many samples we'll output per second
    float samplerate = hardware.AudioSampleRate();

    // Create an ADC configuration
    AdcChannelConfig adcConfig;

    // Add pin 21 as an analog input in this config. We'll use this to read the knob
    adcConfig.InitSingle(hardware.GetPin(21));

    // Set the ADC to use our configuration
    hardware.adc.Init(&adcConfig, 1);

    banks_init(&banks, NUM_OSC);
    banks_setFreq(&banks, 440);
    banks_setMult(&banks, 0.5);

    // Set up oscillators and LFOs
    for (int i = 0; i < NUM_OSC; i++)
    {
        osc[i].Init(samplerate);
        osc[i].SetWaveform(osc[i].WAVE_SIN);
        osc[i].SetAmp(0.5f / (float)NUM_OSC);
        osc[i].SetFreq(banks.freq[i]);

        lfo[i].Init(samplerate);
        lfo[i].SetWaveform(lfo[i].WAVE_SIN);
        lfo[i].SetAmp(0.5f / (float)NUM_OSC);
        lfo[i].SetFreq(lfoArray[i]);
    }

    // Start the adc
    hardware.adc.Start();

    //Start calling the audio callback
    hardware.StartAudio(AudioCallback);

    // Loop forever
    for (;;)
    {
    }
}