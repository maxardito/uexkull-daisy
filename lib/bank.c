#include "bank.h"
#include "lut.h"

// For a vectorized oscillator LUT
#include "../libDaisy/Drivers/CMSIS/DSP/Include/arm_math.h"

#define TEST_LFO_FREQ 0.01

void BK_init(bank_t *self,
             uint8_t numOsc,
             float sampleRate,
             float fund,
             float mult,
             waveform_t waveform)
{
    self->_numOsc = numOsc;

    self->lut_ids = (float *)calloc(numOsc, sizeof(float));
    self->frequencies = (float *)calloc(numOsc, sizeof(float));
    self->_invHalfSampleRate = (float *)calloc(numOsc, sizeof(float));

    self->osc = (osc_t *)malloc(sizeof(osc_t) * self->_numOsc);
    //self->lfo = (osc_t *)malloc(sizeof(osc_t) * self->_numOsc);

    self->_sampleRate = sampleRate;
    self->_fund = fund;
    self->_mult = mult;
    /*modular_t _modular;*/
    self->_1vo = 0;

    self->waveform = SIN;

    for (int i = 0; i < numOsc; i++)
    {
        self->osc[i] = *osc_init();
        self->_invHalfSampleRate[i] = 2.0f / sampleRate;
        self->_lutHalf[i] = LUT_SIZE / 2;
        //    self->lfo[i] = *osc_init();
    }
}

static float _lim_f_n1_1(float in)
{
    return (in < -1.0f ? -1.0f : in > 1.0f ? 1.0f : in);
}

void BK_setFrequencyVectors(bank_t *self, float *vector, uint8_t numFreq)
{

    arm_mult_f32(self->_invHalfSampleRate, vector, self->frequencies, self->_numOsc);

    for (int i = 0; i < numFreq; i++)
    {
        // osc_time(&(self->osc[i]), self->frequencies[i]);
        self->frequencies[i] = _lim_f_n1_1(self->frequencies[i]);
    }
}

static float BK_osc_step(bank_t *self,
                         int index,
                         float *fbase,
                         uint16_t *base,
                         float *baseAsFloat,
                         float *lut,
                         float *lutOffset)
{
    self->lut_ids[index] = self->lut_ids[index] + self->frequencies[index];
    if (self->lut_ids[index] >= 2.0)
    {
        self->lut_ids[index] -= 2.0;
    }
    // lookup table w/ linear interpolation
    // float fbase = (float)LUT_SIN_HALF * self->lut_ids[index];
    // uint16_t base = (uint16_t)fbase[index];
    float mix = fbase[index] - baseAsFloat[index];
    // float lut = sine_lut[base[index]];
    float lut_i = lut[index];
    return (lut_i + mix * (lutOffset[index] - lut_i));
}

float BK_process(bank_t *self)
{
    float sig = 0;

    float fbase[self->_numOsc], baseAsFloat[self->_numOsc], mix[self->_numOsc];
    float lut[self->_numOsc], lutOffset[self->_numOsc];
    uint16_t base[self->_numOsc];

    arm_mult_f32(self->_lutHalf, self->lut_ids, fbase, self->_numOsc);

    for (int i = 0; i < self->_numOsc; i++)
    {
        base[i] = (uint16_t)fbase[i];
        baseAsFloat[i] = base[i];
        lut[i] = cos_lut[base[i]];
        lutOffset[i] = cos_lut[base[i] + 1];
        self->lut_ids[i] += self->frequencies[i];

        if (self->lut_ids[i] > 2)
            self->lut_ids[i] -= 2;
    }

    // for (int i = 0; i < self->_numOsc; i++)
    // {
    //     sig += BK_osc_step(self, i, fbase, base, baseAsFloat, lut, lutOffset);
    // }

    arm_sub_f32(fbase, baseAsFloat, mix, self->_numOsc);
    arm_sub_f32(lutOffset, lut, fbase, self->_numOsc);
    arm_mult_f32(mix, fbase, baseAsFloat, self->_numOsc);
    arm_add_f32(lut, baseAsFloat, fbase, self->_numOsc);

    for (int i = 0; i < self->_numOsc; i++)
    {
        sig += fbase[i];
    }

    sig /= self->_numOsc;

    return sig;
}