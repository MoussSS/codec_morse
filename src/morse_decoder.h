#ifndef _MORSE_DECODER_H_
#define _MORSE_DECODER_H_

#include <stdint.h>
#include <stdbool.h>


typedef struct morse_decoder_s {
    uint16_t counter;
} morse_decoder_t;


void morse_decoder_initialize_(morse_decoder_t* this);
void decode_morse_signal(morse_decoder_t* this, bool signal_is_on);

#endif //_MORSE_DECODER_H_