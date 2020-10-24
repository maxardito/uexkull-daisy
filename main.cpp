#include "daisy_seed.h"
#include "daisysp.h"
#include "uexkull.h"
//#include "cuteop.h"

using namespace daisy;
using namespace daisysp;

Oscillator lfo;

AnalogControl knob1;
Parameter mult;

// Declare a DaisySeed object called hardware
DaisySeed hardware;
uexkull_t uexkull;

void AudioCallback(float *in, float *out, size_t size)
{
    // Fill the block with samples
    for (size_t i = 0; i < size; i += 2)
    {
        float sig = 0;

        sig = UX_process(&uexkull, 1, 440.0f);

        // Set the left and right outputs
        out[i] = sig;
        out[i + 1] = sig;
    }
}

void HardwareInit()
{
    // Configure and Initialize the Daisy Seed
    // These are separate to allow reconfiguration of any of the internal
    // components before initialization.

    hardware.Configure();
    hardware.Init();

    // Create an ADC configuration
    AdcChannelConfig multKnob;
    multKnob.InitSingle(hardware.GetPin(21));
    //knob1.Init(hardware.adc.GetPtr(28), hardware.AudioSampleRate() / 100);
    //mult.Init(knob1, 0.0f, 2.0f, mult.LOGARITHMIC);

    hardware.adc.Init(&multKnob, 1);
}

int main(void)
{
    HardwareInit();

    UX_init(&uexkull, hardware.AudioSampleRate());
    lfo.Init(hardware.AudioSampleRate());
    lfo.SetWaveform(lfo.WAVE_SIN);
    lfo.SetAmp(0.5f);
    lfo.SetFreq(0.5);

    // Start the adc
    hardware.adc.Start();

    //Start calling the audio callback
    hardware.StartAudio(AudioCallback);

    // Loop forever
    for (;;)
    {
    }
}