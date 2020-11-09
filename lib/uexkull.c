#include "uexkull.h"

void UX_init(uexkull_t *self, float samplerate)
{
    BK_init(&(self->bank),
            NUM_OSC,
            samplerate,
            200.0f,
            0.5f,
            SIN);
}

void UX_calculateFreqVector(uexkull_t *self, float fundamental, float mult)
{
    self->freqs[0] = fundamental;

    for (int i = 1; i < NUM_OSC; i++)
    {
        self->freqs[i] = (self->freqs[i - 1] + (self->freqs[i - 1] * mult));

        if (self->freqs[i] > MAX_FREQ)
        {
            self->freqs[i] = 0;
        }
    }
}

float UX_process(uexkull_t *self, float mult, float freq)
{
    float sig = 0;
    UX_calculateFreqVector(self, freq, mult);

    BK_setFrequencyVectors(&(self->bank), self->freqs, NUM_OSC);

    sig += BK_process(&(self->bank));

    return sig;
}