#ifndef _MORSE_ENCODER_H_
#define _MORSE_ENCODER_H_

#include <stdint.h>
#include <stdbool.h>

#define MAX_PART_IN_CHARACTER 5
#define MESSAGE_BUFFER_SIZE   250


typedef enum {
    DOT  = 0,
    DASH = 1
} morse_part_e;


typedef struct encoded_char_s{
    uint8_t nb_of_part;
    morse_part_e char_parts[MAX_PART_IN_CHARACTER];
} encoded_char_t;


typedef struct morse_encoder_s {
    char           buffer[MESSAGE_BUFFER_SIZE];
    bool           morse_signal_is_on;
    uint8_t        last_char_index;
    uint8_t        current_char_index;
    bool           send_char;
    encoded_char_t encoded_char_sending;
    uint8_t        step;
    uint8_t        signal_duration;
    uint8_t        cycle;
} morse_encoder_t;


void morse_encoder_initialize_(morse_encoder_t* this);
void push_character(morse_encoder_t* this, char pushed_char);
void encode_morse_message(morse_encoder_t* this);

inline bool morse_signal_to_transmit_is_on(morse_encoder_t* this) {
    return this->morse_signal_is_on;
}

#endif //_MORSE_ENCODER_H_