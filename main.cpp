#include "daisy_seed.h"
#include "daisysp.h"

#define NUM_OSC 6

using namespace daisy;
using namespace daisysp;

// Declare a DaisySeed object called hardware
DaisySeed hardware;
Oscillator osc[NUM_OSC];
Oscillator lfo[NUM_OSC];

void AudioCallback(float *in, float *out, size_t size)
{
    float sig = 0;

    // Fill the block with samples
    for (size_t i = 0; i < size; i += 2)
    {
        // out[i] = 0;
        // out[i + 1] = 0;

        // Get next osc sample
        // NOTE: pot controlling frequency -
        // osc[0].SetFreq(mtof(hardware.adc.GetFloat(0) * 127));
        for (int j = 0; j < NUM_OSC; j++)
        {
            sig += osc[j].Process();
        }
        // Set the left and right outputs
        out[i] = sig;
        out[i + 1] = sig;

        sig = 0;
    }
}

int main(void)
{
    float freqArray[NUM_OSC] = {440, 900, 660, 990, 100, 800};

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

    // Create LFOs

    // Set up oscillator
    for (int i = 0; i < NUM_OSC; i++)
    {
        osc[i].Init(samplerate);
        osc[i].SetWaveform(osc[i].WAVE_SIN);
        osc[i].SetAmp(0.5f / (float)NUM_OSC);
        osc[i].SetFreq(freqArray[i]);
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