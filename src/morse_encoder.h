#ifndef _MORSE_ENCODER_H_
#define _MORSE_ENCODER_H_

#include <stdint.h>
#include <stdbool.h>

#define MESSAGE_BUFFER_SIZE   250
#define MAX_SEQUENCE_STEPS    9


typedef struct signal_sequence_s {
    uint8_t nb_of_steps;
    uint8_t step_nb_of_cycles[MAX_SEQUENCE_STEPS];
    bool    start_state_is_on;
    uint8_t step;
    uint8_t cycle;
} signal_sequence_t;

typedef struct morse_encoder_s {
    char              buffer[MESSAGE_BUFFER_SIZE];
    bool              morse_signal_is_on;
    uint8_t           last_char_index;
    uint8_t           current_char_index;
    bool              sending_char;
    signal_sequence_t signal_sequence;
} morse_encoder_t;


void morse_encoder_initialize_(morse_encoder_t* this);
void push_character(morse_encoder_t* this, char pushed_char);
void encode_morse_message(morse_encoder_t* this);

inline bool morse_signal_to_transmit_is_on(morse_encoder_t* this) {
    return this->morse_signal_is_on;
}

#endif //_MORSE_ENCODER_H_