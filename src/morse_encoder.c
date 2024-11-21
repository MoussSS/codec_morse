#include "morse_encoder.h"


/* CONSTANTS */
#define TIME_TO_CYCLE_FACTOR 10
#define MORSE_REFERENCE_DURATION_MS 150

static const uint8_t DOT_NB_OF_CYCLE = MORSE_REFERENCE_DURATION_MS / TIME_TO_CYCLE_FACTOR;
static const uint8_t DASH_NB_OF_CYCLE = DOT_NB_OF_CYCLE * 3;
static const uint8_t SPACE_BETWEEN_PARTS_NB_OF_CYCLE = DOT_NB_OF_CYCLE;
static const uint8_t SPACE_BETWEEN_LETTERS_NB_OF_CYCLE = DOT_NB_OF_CYCLE * 3;
static const uint8_t SPACE_BETWEEN_WORDS_NB_OF_CYCLE = DOT_NB_OF_CYCLE * 7;

const encoded_char_t ENCODED_NUMBERS[10] = {
    {5, {DASH, DASH, DASH, DASH, DASH}},  // 0
    {5, {DOT,  DASH, DASH, DASH, DASH}},  // 1
    {5, {DOT,  DOT,  DASH, DASH, DASH}},  // 2
    {5, {DOT,  DOT,  DOT,  DASH, DASH}},  // 3
    {5, {DOT,  DOT,  DOT,  DOT,  DASH}},  // 4
    {5, {DOT,  DOT,  DOT,  DOT,  DOT }},  // 5
    {5, {DASH, DOT,  DOT,  DOT,  DOT }},  // 6
    {5, {DASH, DASH, DOT,  DOT,  DOT }},  // 7
    {5, {DASH, DASH, DASH, DOT,  DOT }},  // 8
    {5, {DASH, DASH, DASH, DASH, DOT }}   // 9
};

const encoded_char_t ENCODED_LETTERS[26] = {
    {2, {DOT,  DASH,                 }},  // A
    {4, {DASH, DOT,  DOT,  DOT,      }},  // B
    {4, {DASH, DOT,  DASH, DOT,      }},  // C
    {3, {DASH, DOT,  DOT,            }},  // D
    {1, {DOT,                        }},  // E
    {4, {DOT,  DOT,  DASH, DOT,      }},  // F
    {3, {DASH, DASH, DOT,            }},  // G
    {4, {DOT,  DOT,  DOT,  DOT,      }},  // H
    {2, {DOT,  DOT,                  }},  // I
    {4, {DOT,  DASH, DASH, DASH,     }},  // J
    {3, {DASH, DOT,  DASH,           }},  // K
    {4, {DOT,  DASH, DOT,  DOT,      }},  // L
    {2, {DASH, DASH,                 }},  // M
    {2, {DASH, DOT,                  }},  // N
    {3, {DASH, DASH, DASH,           }},  // O
    {4, {DOT,  DASH, DASH, DOT,      }},  // P
    {4, {DASH, DASH, DOT,  DASH,     }},  // Q
    {3, {DOT,  DASH, DOT,            }},  // R
    {3, {DOT,  DOT,  DOT,            }},  // S
    {1, {DASH                        }},  // T
    {3, {DOT,  DOT,  DASH,           }},  // U
    {4, {DOT,  DOT,  DOT,  DASH,     }},  // V
    {3, {DOT,  DASH, DASH,           }},  // W
    {4, {DASH, DOT,  DOT,  DASH,     }},  // X
    {4, {DASH, DOT,  DASH, DASH,     }},  // Y
    {4, {DASH, DASH, DOT,  DOT,      }},  // Z  
};

const encoded_char_t ENCODED_NULL_CHAR = {0, {0, 0, 0, 0, 0}};


/* METHODS */
void morse_encoder_initialize_(morse_encoder_t* this) {
    this->buffer[0]            = 0;
    this->morse_signal_is_on   = false;
    this->last_char_index      = 0;
    this->current_char_index   = 0;
    this->send_char            = false;
    this->encoded_char_sending = ENCODED_NULL_CHAR;
    this->step                 = 0;
    this->signal_duration      = 0;
    this->cycle                = 0;
}


static uint8_t increment_circular_index(uint8_t index, uint8_t size) {
    uint8_t incremented_index = index + 1;
    if (incremented_index >= size) {
        incremented_index = 0;
    }
    return incremented_index;
}


static bool encode_morse_blank(morse_encoder_t* this) {
    this->morse_signal_is_on = false;
    this->cycle++;
    if (this->cycle >= this->signal_duration) {
        this->cycle = 0;
    }

    return (this->cycle > 0);
}


static bool encode_morse_data(morse_encoder_t* this) {
    const uint8_t PART_TO_DURATION[2] = {DOT_NB_OF_CYCLE, DASH_NB_OF_CYCLE};

    
    if (this->step < (this->encoded_char_sending.nb_of_part * 2)) {

        if (!(this->step & 0x1)) { // even step (signal on)
            this->signal_duration = PART_TO_DURATION[this->encoded_char_sending.char_parts[(this->step / 2)]];
            this->morse_signal_is_on = true;
        } else {             // odd step (signal off)
            this->signal_duration = SPACE_BETWEEN_PARTS_NB_OF_CYCLE;
            this->morse_signal_is_on = false;
        }

        this->cycle++;
        if (this->cycle >= this->signal_duration) {
            this->cycle = 0;
            this->step++;
            if (this->step >= ((this->encoded_char_sending.nb_of_part * 2) - 1)) {
                this->step = 0;
                this->encoded_char_sending.nb_of_part = 0;
            }
        }
    }

    return (this->step < (this->encoded_char_sending.nb_of_part * 2));
}


static bool send_character (morse_encoder_t* this, char char_to_send) {
            if ((char_to_send >= '0') && (char_to_send <= '9')) {
                this->encoded_char_sending = ENCODED_NUMBERS[char_to_send - '0'];
            } else if ((char_to_send >= 'A') && (char_to_send <= 'Z')) {
                this->encoded_char_sending = ENCODED_LETTERS[char_to_send - 'A'];
            } else if ((char_to_send >= 'a') && (char_to_send <= 'z')) {
                this->encoded_char_sending = ENCODED_LETTERS[char_to_send - 'a'];
            }

    return encode_morse_data(this);
}


void push_character(morse_encoder_t* this, char pushed_char) {
    this->buffer[this->last_char_index] = pushed_char;
    this->last_char_index = increment_circular_index(this->last_char_index, MESSAGE_BUFFER_SIZE);
}

void encode_morse_message(morse_encoder_t* this) {
    bool busy;

    if (this->send_char) {
        busy = send_character(this, this->buffer[this->current_char_index]);
    } else {
        busy = encode_morse_blank(this);
    }

    if (!busy) {
        if (this->send_char) {
            this->send_char = false;
            this->current_char_index = increment_circular_index(this->current_char_index, MESSAGE_BUFFER_SIZE);
            if (this->buffer[this->current_char_index] == ' ') {
                this->signal_duration = SPACE_BETWEEN_WORDS_NB_OF_CYCLE;
                this->current_char_index = increment_circular_index(this->current_char_index, MESSAGE_BUFFER_SIZE);
            } else {
                this->signal_duration = SPACE_BETWEEN_LETTERS_NB_OF_CYCLE;
             }
        } else {
            if (this->current_char_index != this->last_char_index)
                this->send_char = true;
        }
    }
}
