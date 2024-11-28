#include "morse_encoder.h"
#include "util.h"


/* CONSTANTS AND TYPES */
static const uint8_t DOT_NB_OF_CYCLE = MORSE_REFERENCE_DURATION_MS / TIME_TO_CYCLE_FACTOR;
static const uint8_t DASH_NB_OF_CYCLE = DOT_NB_OF_CYCLE * 3;
static const uint8_t SPACE_BETWEEN_PARTS_NB_OF_CYCLE = DOT_NB_OF_CYCLE;
static const uint8_t SPACE_BETWEEN_LETTERS_NB_OF_CYCLE = DOT_NB_OF_CYCLE * 3;
static const uint8_t SPACE_BETWEEN_WORDS_NB_OF_CYCLE = DOT_NB_OF_CYCLE * 7;

const encoded_char_t    ENCODED_NULL_CHAR  = {0, {0, 0, 0, 0, 0}};
const signal_sequence_t NO_SIGNAL_SEQUENCE = {
    0,                           // nb_of_steps
    {0, 0, 0, 0, 0, 0, 0, 0, 0}, // step_nb_of_cycles
    false,                       // start_state_is_on
    0,                           // step
    0,                           // cycle
};


/* PRIVATE METHODS DECLARATIONS */
static void encode_blank_sequence(signal_sequence_t* sequence, bool is_a_word_separator);
static void encode_character_sequence(signal_sequence_t* sequence, char char_to_send);
static bool find_next_alphanumerical_char(morse_encoder_t* this);
static void run_signal_sequence(morse_encoder_t* this);


/* INLINE FUNCTIONS */
static inline bool char_is_alphanumerical(char character) {
    return (
        ((character >= '0') && (character <= '9')) ||
        ((character >= 'A') && (character <= 'Z')) || 
        ((character >= 'a') && (character <= 'z')));
}


static inline bool signal_sequence_is_in_progress(morse_encoder_t* this) {
    return (this->signal_sequence.nb_of_steps > 0);
}


/* PUBLIC METHODS */
void morse_encoder_initialize_(morse_encoder_t* this) {
    this->buffer[0]          = 0;
    this->morse_signal_is_on = false;
    this->last_char_index    = 0;
    this->current_char_index = 0;
    this->sending_char       = false;
    this->signal_sequence    = NO_SIGNAL_SEQUENCE;
}


void push_character(morse_encoder_t* this, char pushed_char) {
    this->buffer[this->last_char_index] = pushed_char;
    this->last_char_index = increment_circular_index(this->last_char_index, MESSAGE_BUFFER_SIZE);
    this->buffer[this->last_char_index] = ' '; // Force a long blanck when the last character is reached
}


void encode_morse_message(morse_encoder_t* this) {
    if (!signal_sequence_is_in_progress(this)) {
        if (this->sending_char) {
            // Was sending the character sequence, send the blanck sequence
            this->sending_char = false;
            if (find_next_alphanumerical_char(this)) {
                // Next character is alphanumerical
                encode_blank_sequence(&this->signal_sequence, false);
            } else {
                // Next character is another character
                encode_blank_sequence(&this->signal_sequence, true);
             }
        } else {
            // Was sending the blanck sequence between characters or words, send the next character
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


static bool find_next_alphanumerical_char(morse_encoder_t* this) {
    bool next_character_is_alphanumerical = true;
    bool character_is_alphanumerical = false;

    this->current_char_index = increment_circular_index(this->current_char_index, MESSAGE_BUFFER_SIZE);
    character_is_alphanumerical = char_is_alphanumerical(this->buffer[this->current_char_index]);
    while ((!character_is_alphanumerical) && (this->current_char_index != this->last_char_index)) {
        this->current_char_index = increment_circular_index(this->current_char_index, MESSAGE_BUFFER_SIZE);
        character_is_alphanumerical = char_is_alphanumerical(this->buffer[this->current_char_index]);
        next_character_is_alphanumerical = false;
    };

    return next_character_is_alphanumerical;
}


static void run_signal_sequence(morse_encoder_t* this) {
    signal_sequence_t* sequence = &this->signal_sequence;

    // Update signal on new signal step
    if (sequence->cycle == 0) {
        if (sequence->step == 0) {
            this->morse_signal_is_on = sequence->start_state_is_on;
        } else {
		    this->morse_signal_is_on = !this->morse_signal_is_on;
        }
    }

    sequence->cycle++;
    if (sequence->cycle >= sequence->step_nb_of_cycles[sequence->step]) {
        // Step is over
        sequence->cycle = 0;
        sequence->step++;
        if (sequence->step >= sequence->nb_of_steps) {
            // Sequence is over (reset)
            *sequence = NO_SIGNAL_SEQUENCE;
        }
    }
}