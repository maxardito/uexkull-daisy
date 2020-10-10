#include "uexkull.h"

void UX_init(uexkull_t *self, float samplerate)
{
    for (int i = 0; i < NUM_BANKS; i++)
    {

        BK_init(&(self->centralBanks[i]),
                NUM_OSC,
                samplerate,
                440.0f,
                1.0f,
                WAVE_SAW);

        BK_init(&(self->reactiveBanks[i]),
                NUM_OSC,
                samplerate,
                440.0f,
                1.0f,
                WAVE_SAW);
    }
}

float UX_process(uexkull_t *self)
{
    float sig = 0;

    for (int i = 0; i < NUM_BANKS; i++)
    {
        sig += (BK_process(&(self->centralBanks[i])) / NUM_BANKS);
    }

    return sig;
}