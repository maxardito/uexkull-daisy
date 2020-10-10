#include "daisy_seed.h"
#include "daisysp.h"
#include "uexkull.h"
//#include "cuteop.h"

using namespace daisy;
using namespace daisysp;

// Declare a DaisySeed object called hardware
DaisySeed hardware;
uexkull_t uexkull;

void AudioCallback(float *in, float *out, size_t size)
{
    // Fill the block with samples
    for (size_t i = 0; i < size; i += 2)
    {
        float sig = 0;

        sig = UX_process(&uexkull);

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
    AdcChannelConfig adcConfig;

    // Add pin 21 as an analog input in this config. We'll use this to read the knob
    adcConfig.InitSingle(hardware.GetPin(28));

    // Set the ADC to use our configuration
    hardware.adc.Init(&adcConfig, 1);
}

int main(void)
{
    HardwareInit();

    UX_init(&uexkull, hardware.AudioSampleRate());

    // Start the adc
    hardware.adc.Start();

    //Start calling the audio callback
    hardware.StartAudio(AudioCallback);

    // Loop forever
    for (;;)
    {
    }
}