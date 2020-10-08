/**
 * bank.h —— (Max Ardito, 10/07/20)
 *
 * A single oscillator bank...
 */

#ifndef BANK_H
#define BANK_H

using namespace daisysp;

#include <cuteop.h>
#include "globals.h"
#include "daisysp.h"

/**
 * waveform_t: Waveforms selection for bank oscillators
 */
typedef enum
{
    SIN,
    SQUARE,
    TRI,
    SAW
} waveform_t;

/**
 * bank_t: Module's main data structure
 */
typedef struct bank
{
    Oscillator *osc;
    Oscillator *lfo;

    float _fund;            // Bank frequency fundamental
    float _mult;            // Bank frequence sequence multiplier value
    /*modular_t _modular;*/ // Interface for FM/PM/AM functions
    float _1vo;             // 1V/Oct input
    waveform_t waveform;    // Waveform for all bank oscillators
} bank_t;

/**
 * Initialize the 'uexkull' struct

 */
void BK_init(bank_t *self,
             uint8_t numOsc,
             float fund,
             float mult,
             waveform_t waveform);

/**
 * Frees the 'uexkull' struct
 *
 * TODO: Add and describe parameters
 */
void BK_destroy(bank_t *self);

#endif /* BANK_H */
