#include "daisy_seed.h"
#include "daisysp.h"
#include "cuteop.h"

extern "C"
{
#include "uexkull.h"
}

#define MULT_POT 21
#define FREQ_POT 20

using namespace daisy;
using namespace daisysp;

// Declare a DaisySeed object called hardware
DaisySeed hardware;
uexkull_t uexkull;

float *channel_array;

void AudioCallback(float *in, float *out, size_t size)
{
    float mult = hardware.adc.GetFloat(0);
    float freq = hardware.adc.GetFloat(1) * 20000;

    // Fill the block with samples
    for (size_t i = 0; i < size; i += 2)
    {
        UX_process(&uexkull, mult, freq);

        // Set the left and right outputs
        out[i] = uexkull.sig[0];
        out[i + 1] = uexkull.sig[1];
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
    AdcChannelConfig knobs[2];
    knobs[0].InitSingle(hardware.GetPin(MULT_POT));
    knobs[1].InitSingle(hardware.GetPin(FREQ_POT));

    hardware.adc.Init(knobs, 2);

    // Start the adc
    hardware.adc.Start();
}

int main(void)
{
    HardwareInit();

    channel_array = (float *)malloc(sizeof(float) * 4);

    UX_init(&uexkull, hardware.AudioSampleRate());

    //Start calling the audio callback
    hardware.StartAudio(AudioCallback);

    // Loop forever
    for (;;)
    {
    }
}