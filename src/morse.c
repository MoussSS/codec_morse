#include <stdint.h>
#include <stdbool.h>
#include "morse.h"

#define TIME_TO_CYCLE_FACTOR 10
#define MORSE_REFERENCE_DURATION_MS 150
#define MAX_PART_IN_CHARACTER 5

static const uint8_t DOT_NB_OF_CYCLE = MORSE_REFERENCE_DURATION_MS / TIME_TO_CYCLE_FACTOR;
static const uint8_t DASH_NB_OF_CYCLE = DOT_NB_OF_CYCLE * 3;
static const uint8_t SPACE_BETWEEN_PARTS_NB_OF_CYCLE = DOT_NB_OF_CYCLE;
static const uint8_t SPACE_BETWEEN_LETTERS_NB_OF_CYCLE = DOT_NB_OF_CYCLE * 3;
static const uint8_t SPACE_BETWEEN_WORDS_NB_OF_CYCLE = DOT_NB_OF_CYCLE * 7;

static uint8_t cycle = 0;
static uint8_t step = 0;

typedef enum {
    DOT  = 0,
    DASH = 1
} morse_part_e;

typedef struct {
    uint8_t nb_of_part;
    morse_part_e char_parts[MAX_PART_IN_CHARACTER];
} encoded_char_s;

const encoded_char_s ENCODED_NUMBERS[10] = {
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

const encoded_char_s ENCODED_LETTERS[26] = {
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

extern void set_morse_signal(bool is_on);
static encoded_char_s encoded_char_sending = {0, {0, 0, 0, 0, 0}}; 
static uint8_t signal_duration = 0;


static bool encode_morse_blank(void) {
    set_morse_signal(false);
    cycle++;
    if (cycle >= signal_duration) {
        cycle = 0;
    }

    return (cycle > 0);
}

static bool encode_morse_data(void) {
    const uint8_t PART_TO_DURATION[2] = {DOT_NB_OF_CYCLE, DASH_NB_OF_CYCLE};

    
    if (step < (encoded_char_sending.nb_of_part * 2)) {

        if (!(step & 0x1)) { // even step (signal on)
            signal_duration = PART_TO_DURATION[encoded_char_sending.char_parts[(step / 2)]];
            set_morse_signal(true);
        } else {             // odd step (signal off)
            signal_duration = SPACE_BETWEEN_PARTS_NB_OF_CYCLE;
            set_morse_signal(false);
        }

        cycle++;
        if (cycle >= signal_duration) {
            cycle = 0;
            step++;
            if (step >= ((encoded_char_sending.nb_of_part * 2) - 1)) {
                step = 0;
                encoded_char_sending.nb_of_part = 0;
            }
        }
    }

    return (step < (encoded_char_sending.nb_of_part * 2));
}

static bool send_char = false;

static bool send_character (char char_to_send) {
            if ((char_to_send >= '0') && (char_to_send <= '9')) {
                encoded_char_sending = ENCODED_NUMBERS[char_to_send - '0'];
            } else if ((char_to_send >= 'A') && (char_to_send <= 'Z')) {
                encoded_char_sending = ENCODED_LETTERS[char_to_send - 'A'];
            } else if ((char_to_send >= 'a') && (char_to_send <= 'z')) {
                encoded_char_sending = ENCODED_LETTERS[char_to_send - 'a'];
            }

    return encode_morse_data();
}

static char message[] = "GROS CAca Boudin";
static uint8_t char_index = 0;

void encode_morse_message(void) {
    bool busy;
   
    if (send_char) {
        busy = send_character(message[char_index]);
    } else {
        busy = encode_morse_blank();
    }

    if (!busy) {
        if (send_char) {
            send_char = false;
            char_index++;
            if (message[char_index] == ' ') {
                signal_duration = SPACE_BETWEEN_WORDS_NB_OF_CYCLE;
                char_index++;
            } else {
                signal_duration = SPACE_BETWEEN_LETTERS_NB_OF_CYCLE;
             }
        } else {
            if (message[char_index] != 0)
                send_char = true;
        }
    }
}
