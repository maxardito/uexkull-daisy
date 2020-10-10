#include "bank.h"

#define TEST_LFO_FREQ 0.01

void BK_init(bank_t *self,
             uint8_t numOsc,
             float sampleRate,
             float fund,
             float mult,
             waveform_t waveform)
{
    self->_numOsc = numOsc;

    self->osc = (Oscillator *)malloc(sizeof(Oscillator) * self->_numOsc);
    self->lfo = (Oscillator *)malloc(sizeof(Oscillator) * self->_numOsc);

    self->_sampleRate = sampleRate;
    self->_fund = fund;
    self->_mult = mult;
    /*modular_t _modular;*/
    self->_1vo = 0;

    self->waveform = WAVE_SIN;

    for (int i = 0; i < numOsc; i++)
    {
        self->osc[i].Init(self->_sampleRate);
        self->osc[i].SetWaveform(self->waveform);
        self->osc[i].SetAmp(0.5f / (float)numOsc);
        self->osc[i].SetFreq(self->_fund);

        self->lfo[i].Init(self->_sampleRate);
        self->lfo[i].SetWaveform(self->waveform);
        self->lfo[i].SetAmp(0.5f);
        self->lfo[i].SetFreq(TEST_LFO_FREQ);
    }
}

float BK_process(bank_t *self)
{
    float sig = 0;

    for (int i = 0; i < self->_numOsc; i++)
    {
        sig += self->osc[i].Process();
    }

    return sig;
}