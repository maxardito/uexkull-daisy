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
        osc_time(&(self->osc[i]), self->frequencies[i]);
        //self->frequencies[i] = _lim_f_n1_1((float)(vector[i] / self->_halfSampleRate));
    }
}

float BK_process(bank_t *self)
{
    float sig = 0;

    for (int i = 0; i < self->_numOsc; i++)
    {
        sig += osc_step(&(self->osc[i]), 0) / self->_numOsc;
    }

    return sig;
}