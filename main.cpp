#include "daisy_seed.h"
#include "daisysp.h"
#include "cuteop.h"

#define NUM_OSC 32

using namespace daisy;
using namespace daisysp;

// Declare a DaisySeed object called hardware
DaisySeed hardware;
Oscillator osc[NUM_OSC];
Oscillator lfo;

t_banks banks;

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
            //banks_setMult(&banks, (double)(lfo.Process() + 0.5) / 3);
            //osc[j].SetFreq(banks.freq[j]);
            sig += (osc[j].Process()); //* lfo[j].Process());
        }
        // Set the left and right outputs
        out[i] = sig;
        out[i + 1] = sig;
    }
}

int main(void)
{
    //float lfoArray[NUM_OSC] = {3, 15, 0.8, 1, 0.43, 0.12};

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
    banks_setMult(&banks, (double)1.0);

    // Set up oscillators and LFOs
    for (int i = 0; i < NUM_OSC; i++)
    {
        osc[i].Init(samplerate);
        osc[i].SetWaveform(osc[i].WAVE_SIN);
        osc[i].SetAmp(0.5f / (float)NUM_OSC);
        osc[i].SetFreq(banks.freq[i]);

        //lfo.Init(samplerate);
        //lfo.SetWaveform(lfo.WAVE_SIN);
        //lfo.SetAmp(0.5f);
        //lfo.SetFreq(0.01);
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