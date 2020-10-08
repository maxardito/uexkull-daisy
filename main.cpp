#include "daisy_seed.h"
#include "daisysp.h"
//#include "cuteop.h"

#define NUM_OSC 8

using namespace daisy;
using namespace daisysp;

// Declare a DaisySeed object called hardware
DaisySeed hardware;
Oscillator osc[NUM_OSC];
Oscillator lfo[NUM_OSC];
float oscArray[NUM_OSC] = {1000, 1500, 1750, 2500, 2700, 4500, 6705, 7250};

void AudioCallback(float *in, float *out, size_t size)
{
    // Fill the block with samples
    for (size_t i = 0; i < size; i += 2)
    {
        float sig = 0;

        // Get next osc sample
        // NOTE: pot controlling frequency -
        // osc[0].SetFreq(mtof(hardware.adc.GetFloat(0) * 127));
        //banks_setFreq(&banks, mtof(hardware.adc.GetFloat(0) * 1));

        for (int j = 0; j < NUM_OSC; j++)
        {
            osc[j].SetFreq(oscArray[j] * (lfo[j].Process() + 1.0f));
            sig += (osc[j].Process()); //* lfo[j].Process());
        }
        // Set the left and right outputs
        out[i] = sig;
        out[i + 1] = sig;
    }
}

int main(void)
{
    float lfoArray[NUM_OSC] = {3, 15, 0.8, 1, 0.43, 0.12, 0.5, 0.23};

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
    adcConfig.InitSingle(hardware.GetPin(28));

    // Set the ADC to use our configuration
    hardware.adc.Init(&adcConfig, 1);

    // Set up oscillators and LFOs
    for (int i = 0; i < NUM_OSC; i++)
    {
        osc[i].Init(samplerate);
        osc[i].SetWaveform(osc[i].WAVE_SIN);
        osc[i].SetAmp(0.5f / (float)NUM_OSC);
        osc[i].SetFreq(oscArray[i]);

        lfo[i].Init(samplerate);
        lfo[i].SetWaveform(lfo[i].WAVE_SIN);
        lfo[i].SetAmp(0.5f);
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