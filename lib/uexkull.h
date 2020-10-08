/**
 * uexkull.h —— (Max Ardito, 10/07/20)
 *
 * Class representing the entire module's functionality. 
 * Instantiated globally in [/main.cpp]. Bridges the gap 
 * between the Daisy's Oscillator classes and Cute-Op's
 * sequence class. Processes frequency ratios in the main
 * loop
 */

#ifndef UEXKULL_H
#define UEXKULL_H

#include <cuteop.h>
#include "globals.h"
#include "bank.h"

/**
 * uexkull_t: Module's main data structure
 */
typedef struct uexkull
{
    bank_t centralBanks[NUM_BANKS];
    bank_t reactiveBanks[NUM_BANKS];
    //sequence_t f;
} uexkull_t;

/**
 * Initialize the 'uexkull' struct

 */
void UX_init(uexkull_t *self,
             bank_t *centralBanks,
             bank_t *reactiveBanks
             /*t_sequence f*/);

/**
 * Frees the 'uexkull' struct
 *
 * TODO: Add and describe parameters
 */
void UX_destroy(uexkull_t *self);

#endif /* OPPORTUNITY_H */
