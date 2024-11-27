#include "morse_decoder.h"


/* CONSTANTS AND TYPES */


/* PRIVATE METHODS DECLARATIONS */


/* INLINE FUNCTIONS */


/* PUBLIC METHODS */
void morse_decoder_initialize_(morse_decoder_t* this) {
    this->counter = 0;
}


void decode_morse_signal(morse_decoder_t* this, bool signal_is_on) {
    if (signal_is_on)
        this->counter++;
}


/* PRIVATE METHODS */
