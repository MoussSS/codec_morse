#ifndef _MORSE_DECODER_H_
#define _MORSE_DECODER_H_

#include <stdint.h>
#include <stdbool.h>


#define NB_OF_SIGNAL_CHANGES_TO_RECORD (6 * 2) // OFF on even indexes / ON on odd indexes

typedef struct morse_decoder_s {
    bool     signal_is_on;
    uint8_t  recording_index;
    uint16_t signal_duration_records[NB_OF_SIGNAL_CHANGES_TO_RECORD];
    uint8_t  decoding_index;
    uint16_t reference_duration;
    char     decoded_character;
    char     additional_character;
} morse_decoder_t;


void morse_decoder_initialize(morse_decoder_t* this);
void decode_morse_signal(morse_decoder_t* this, bool signal_is_on);

inline char get_decoded_character(morse_decoder_t* this) {
    return this->decoded_character;
}


#endif //_MORSE_DECODER_H_