#include "morse_encoder.h"
#include "util.h"


/* CONSTANTS AND TYPES */
#define TIME_TO_CYCLE_FACTOR 10
#define MORSE_REFERENCE_DURATION_MS 150
#define MAX_PART_IN_CHARACTER 5

static const uint8_t DOT_NB_OF_CYCLE = MORSE_REFERENCE_DURATION_MS / TIME_TO_CYCLE_FACTOR;
static const uint8_t DASH_NB_OF_CYCLE = DOT_NB_OF_CYCLE * 3;
static const uint8_t SPACE_BETWEEN_PARTS_NB_OF_CYCLE = DOT_NB_OF_CYCLE;
static const uint8_t SPACE_BETWEEN_LETTERS_NB_OF_CYCLE = DOT_NB_OF_CYCLE * 3;
static const uint8_t SPACE_BETWEEN_WORDS_NB_OF_CYCLE = DOT_NB_OF_CYCLE * 7;

typedef enum {
    DOT  = 0,
    DASH = 1
} morse_part_e;

typedef struct encoded_char_s {
    uint8_t      nb_of_part;
    morse_part_e char_parts[MAX_PART_IN_CHARACTER];
} encoded_char_t;

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

const signal_sequence_t NO_SIGNAL_SEQUENCE = {0, {0, 0, 0, 0, 0, 0, 0, 0, 0}, false};
const encoded_char_t    ENCODED_NULL_CHAR  = {0, {0, 0, 0, 0, 0}};


/* PRIVATE METHODS DECLARATIONS */
static void encode_blank_sequence(signal_sequence_t* sequence, bool is_a_word_separator);
static void encode_character_sequence(signal_sequence_t* sequence, char char_to_send);
static void run_signal_sequence(morse_encoder_t* this);


/* INLINE FUNCTIONS */
static inline bool signal_sequence_is_in_progress(morse_encoder_t* this) {
    return (this->signal_sequence.nb_of_steps > 0);
}


/* PUBLIC METHODS */
void morse_encoder_initialize_(morse_encoder_t* this) { // TODO nettoyer les champs
    this->buffer[0]          = 0;
    this->morse_signal_is_on = false;
    this->last_char_index    = 0;
    this->current_char_index = 0;
    this->sending_char       = false;
    this->signal_sequence    = NO_SIGNAL_SEQUENCE;
    this->step               = 0;
    this->signal_duration    = 0;
    this->cycle              = 0;
}


void push_character(morse_encoder_t* this, char pushed_char) {
    this->buffer[this->last_char_index] = pushed_char;
    this->last_char_index = increment_circular_index(this->last_char_index, MESSAGE_BUFFER_SIZE);
}


void encode_morse_message(morse_encoder_t* this) {
    if (!signal_sequence_is_in_progress(this)) { // TODO commenter (vérifier les cas pourris avec les caractères moisis (retourner un bool pour si c'est pas bon)
        if (this->sending_char) {    // Was sending the character sequence
            this->sending_char = false;
            this->current_char_index = increment_circular_index(this->current_char_index, MESSAGE_BUFFER_SIZE);
            if (this->buffer[this->current_char_index] == ' ') {
                encode_blank_sequence(&this->signal_sequence, true);
                this->current_char_index = increment_circular_index(this->current_char_index, MESSAGE_BUFFER_SIZE);
            } else {
                encode_blank_sequence(&this->signal_sequence, false);
             }
        } else {                     // Was sending the blanck sequence (between characters or words)
            if (this->current_char_index != this->last_char_index) {
				encode_character_sequence(&this->signal_sequence, this->buffer[this->current_char_index]);
                this->sending_char = true;
			}
        }
    }
	
    if (signal_sequence_is_in_progress(this))
        run_signal_sequence(this);
}


/* PRIVATE METHODS */
static void encode_blank_sequence(signal_sequence_t* sequence, bool is_a_word_separator) {
    sequence->nb_of_steps = 1;
    if (is_a_word_separator)
        sequence->step_nb_of_cycles[0] = SPACE_BETWEEN_WORDS_NB_OF_CYCLE;
    else
        sequence->step_nb_of_cycles[0] = SPACE_BETWEEN_LETTERS_NB_OF_CYCLE;
    sequence->start_state_is_on = false;
}


static void encode_character_sequence(signal_sequence_t* sequence, char char_to_send) {
    const uint8_t  PART_TO_DURATION[2] = {DOT_NB_OF_CYCLE, DASH_NB_OF_CYCLE};
    encoded_char_t encoded_char        = ENCODED_NULL_CHAR;

    // Identify the character code
    if ((char_to_send >= '0') && (char_to_send <= '9')) {
        encoded_char = ENCODED_NUMBERS[char_to_send - '0'];
    } else if ((char_to_send >= 'A') && (char_to_send <= 'Z')) {
        encoded_char = ENCODED_LETTERS[char_to_send - 'A'];
    } else if ((char_to_send >= 'a') && (char_to_send <= 'z')) {
        encoded_char = ENCODED_LETTERS[char_to_send - 'a'];
    }

    // Build the signal sequence if the character is encodable
    if (encoded_char.nb_of_part > 0) {
        sequence->nb_of_steps = (2 * encoded_char.nb_of_part) - 1;
        for (uint8_t index = 0; index < encoded_char.nb_of_part - 1; index++) {
            sequence->step_nb_of_cycles[index * 2] = PART_TO_DURATION[encoded_char.char_parts[index]];
            sequence->step_nb_of_cycles[(index * 2) + 1] = SPACE_BETWEEN_PARTS_NB_OF_CYCLE;
        }
        sequence->step_nb_of_cycles[(encoded_char.nb_of_part - 1) * 2] =
            PART_TO_DURATION[encoded_char.char_parts[encoded_char.nb_of_part - 1]];
        sequence->start_state_is_on = true;
    }
}


static void run_signal_sequence(morse_encoder_t* this) {

    // Update signal on new signal step
    if (this->cycle == 0) {
        if (this->step == 0) {
            this->morse_signal_is_on = this->signal_sequence.start_state_is_on;
        } else {
		    this->morse_signal_is_on = !this->morse_signal_is_on;
        }
    }

    this->cycle++;
    if (this->cycle >= this->signal_sequence.step_nb_of_cycles[this->step]) {
        // Step is over
        this->cycle = 0;
        this->step++;
        if (this->step >= this->signal_sequence.nb_of_steps) {
            // Sequence is over
            this->step = 0;
            this->signal_sequence = NO_SIGNAL_SEQUENCE;
        }
    }
}