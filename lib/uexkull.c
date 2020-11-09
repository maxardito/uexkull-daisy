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

void UX_calculateUmweltVector(uexkull_t *self, float mult)
{
    for (int i = 0; i < NUM_OSC; i++)
    {
        self->freqs[i] += (self->freqs[i] * mult);
    }
}

void UX_process(uexkull_t *self, float mult, float freq)
{
    // Zero out artifacts
    self->sig[0] = 0;

    // Calculate main frequency vector and assign to bank
    UX_calculateFreqVector(self, freq, mult);

    BK_setFrequencyVectors(&(self->bank), self->freqs, NUM_OSC);

    self->sig[0] += BK_process(&(self->bank));

    // Calculate remaining Umwelt complementary outputs
    for (int i = 1; i < NUM_OUTPUTS; i++)
    {
        // Zero out artifacts
        self->sig[i] = 0;

        // TODO: Calculate frequency vector as complementary to main bank,
        // rather than a unique frequency array
        UX_calculateUmweltVector(self, (float)(i / NUM_OUTPUTS));
        BK_setFrequencyVectors(&(self->bank), self->freqs, NUM_OSC);

        self->sig[i] += BK_process(&(self->bank));
    }
}
