#include "uexkull.h"

void UX_init(uexkull_t *self, float samplerate)
{
    for (int i = 0; i < NUM_OUTPUTS; i++)
    {
        BK_init(&(self->bank[i]),
                NUM_OSC,
                samplerate,
                200.0f,
                0.5f,
                SIN);

        self->sig[i] = 0;
    }
}

static void _UX_calculateFreqVector(uexkull_t *self, float fundamental, float mult)
{
    self->freqs[0][0] = fundamental;

    for (int i = 1; i < NUM_OSC; i++)
    {
        self->freqs[0][i] = (self->freqs[0][i - 1] + (self->freqs[0][i - 1] * mult));

        if (self->freqs[0][i] > MAX_FREQ)
        {
            self->freqs[0][i] = 0;
        }
    }
}

static void _UX_calculateUmweltVector(uexkull_t *self, int outputIndex, float mult)
{
    for (int i = 0; i < NUM_OSC; i++)
    {
        self->freqs[outputIndex][i] = self->freqs[0][i] + (self->freqs[0][i] * mult);
        if (self->freqs[outputIndex][i] > MAX_FREQ)
        {
            self->freqs[outputIndex][i] = 0;
        }
    }
}

void UX_process(uexkull_t *self, float mult, float freq)
{
    // Calculate main frequency vector and assign to bank
    _UX_calculateFreqVector(self, freq, mult);

    BK_setFrequencyVectors(&(self->bank[0]), self->freqs[0], NUM_OSC);

    self->sig[0] = BK_process(&(self->bank[0]));

    // Calculate remaining Umwelt complementary outputs
    for (int i = 1; i < NUM_OUTPUTS; i++)
    {
        _UX_calculateUmweltVector(self, i, (float)i / NUM_OUTPUTS);
        BK_setFrequencyVectors(&(self->bank[i]), self->freqs[i], NUM_OSC);
        self->sig[i] = BK_process(&(self->bank[i]));
    }
}
