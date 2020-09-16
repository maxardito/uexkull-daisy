#include "daisy_seed.h"
#include "daisysp.h"

#define NUM_OSC 64

// Use the daisy namespace to prevent having to type
// daisy:: before all libdaisy functions
using namespace daisy;
using namespace daisysp;

// Declare a DaisySeed object called hardware
DaisySeed hardware;
Oscillator osc[NUM_OSC];
AdEnv env;

float fund = 200;

Switch button1;

void AudioCallback(float *in, float *out, size_t size)
{
    float osc_out, env_out;

    //Nobody likes a bouncy button
    button1.Debounce();

    //If you push the button,...
    if (button1.RisingEdge())
    {
        env.Trigger(); //Trigger the envelope!
    }

    //Convert floating point knob to midi (0-127)
    //Then convert midi to freq. in Hz
    //osc.SetFreq(mtof(hardware.adc.GetFloat(0) * 127));

    //Fill the block with samples
    for (size_t i = 0; i < size; i += 2)
    {
        //Get the next envelope value
        env_out = env.Process();
        for (int i = 0; i < NUM_OSC; i++)
        {
            //Set the oscillator volume to the latest env value
            osc[i].SetAmp(env_out);
            //get the next oscillator sample
            osc_out = osc[i].Process();
        }

        //Set the left and right outputs
        out[i] = osc_out;
        out[i + 1] = osc_out;
    }
}

int main(void)
{
    // Configure and Initialize the Daisy Seed
    // These are separate to allow reconfiguration of any of the internal
    // components before initialization.
    hardware.Configure();
    hardware.Init();
    ;

    //How many samples we'll output per second
    float samplerate = hardware.AudioSampleRate();

    //Create an ADC configuration
    AdcChannelConfig adcConfig;
    //Add pin 21 as an analog input in this config. We'll use this to read the knob
    adcConfig.InitSingle(hardware.GetPin(21));

    //Initialize the button on pin 28
    button1.Init(hardware.GetPin(28), samplerate / 48.f);

    //Set the ADC to use our configuration
    hardware.adc.Init(&adcConfig, 1);

    //Set up oscillator
    for (int i = 0; i < NUM_OSC; i++)
    {
        osc[i].Init(samplerate);
        osc[i].SetWaveform(osc[i].WAVE_SIN);
        osc[i].SetAmp(1.f / (float)NUM_OSC);
        osc[i].SetFreq(fund);
        fund += (fund * 0.5);
    }

    //Set up volume envelope
    env.Init(samplerate);
    //Envelope attack and decay times
    env.SetTime(ADENV_SEG_ATTACK, .01);
    env.SetTime(ADENV_SEG_DECAY, .4);
    //minimum and maximum envelope values
    env.SetMin(0.0);
    env.SetMax(1.f);
    env.SetCurve(0); // linear

    //Start the adc
    hardware.adc.Start();

    //Start calling the audio callback
    hardware.StartAudio(AudioCallback);

    // Loop forever
    for (;;)
    {
    }
}