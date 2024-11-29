#include "morse_decoder.h"
#include "util.h"


/* CONSTANTS AND TYPES */
#define MAX_DURATION_TO_RECORD 1000 // 10s max


/* PRIVATE METHODS DECLARATIONS */
static void decode_blank(morse_decoder_t* this);
static void decode_character(morse_decoder_t* this);
static char decode_morse_character(encoded_char_t encoded_char);
static char decode_morse_letter(encoded_char_t encoded_char);
static char decode_morse_number(encoded_char_t encoded_char);
static void decode_signal(morse_decoder_t* this);


/* INLINE FUNCTIONS */
static inline bool is_decoding_a_character(morse_decoder_t* this) {
    return !(this->decoding_index & 0x1);
}


static inline bool is_receiving_an_off_signal(morse_decoder_t* this) {
    return !(this->recording_index & 0x1);
}


/* PUBLIC METHODS */
void morse_decoder_initialize(morse_decoder_t* this) {
    this->signal_is_on               = false;
    this->recording_index            = 0;
    this->signal_duration_records[0] = 0;
    this->decoding_index             = 0;
    this->reference_duration         = 15;
    this->decoded_character          = 0;
    this->additional_character       = 0;
}


void decode_morse_signal(morse_decoder_t* this, bool signal_is_on) {

    this->decoded_character = 0;

    if (this->signal_is_on != signal_is_on) {
        // On signal change
        this->recording_index = increment_circular_index(this->recording_index, NB_OF_SIGNAL_CHANGES_TO_RECORD);
        this->signal_duration_records[this->recording_index] = 1;
	    this->signal_is_on = signal_is_on;
    } else {
        // When signal remains constant
        if (this->signal_duration_records[this->recording_index] < MAX_DURATION_TO_RECORD) {
            this->signal_duration_records[this->recording_index]++;
        }
    }

    decode_signal(this);
}


/* PRIVATE METHODS */
static void decode_blank(morse_decoder_t* this) {
    uint8_t new_decoding_index = increment_circular_index(this->decoding_index, NB_OF_SIGNAL_CHANGES_TO_RECORD);
    this->decoded_character = 0;
    
    if (new_decoding_index == this->recording_index) {
        if (this->signal_duration_records[new_decoding_index] > (10 * this->reference_duration)) {
            this->decoded_character = '\n';
            this->additional_character = '\r';
            this->decoding_index = new_decoding_index;
        }
    } else {
        if (this->signal_duration_records[new_decoding_index] > (5 * this->reference_duration))
            this->decoded_character = ' ';
        this->decoding_index = new_decoding_index;
    }
}


static void decode_character(morse_decoder_t* this) {
    uint8_t new_decoding_index = this->decoding_index;
    encoded_char_t encoded_char = {0, {0, 0, 0, 0, 0}};

    do {
        new_decoding_index = increment_circular_index(new_decoding_index, NB_OF_SIGNAL_CHANGES_TO_RECORD);
        if (this->signal_duration_records[new_decoding_index] > (2 * this->reference_duration)) {
            encoded_char.char_parts[encoded_char.nb_of_part] = DASH;
        } else {
            encoded_char.char_parts[encoded_char.nb_of_part] = DOT;
        }
        encoded_char.nb_of_part++;
        this->decoding_index = new_decoding_index;
        new_decoding_index = increment_circular_index(new_decoding_index, NB_OF_SIGNAL_CHANGES_TO_RECORD);
    } while(new_decoding_index != this->recording_index);
    // TODO invalid char if there is more than 5 parts

    this->decoded_character = decode_morse_character(encoded_char);
}


static char decode_morse_character(encoded_char_t encoded_char) {
    char decoded_char = 0;

    if (encoded_char.nb_of_part == 5) {
        decoded_char = decode_morse_number(encoded_char);
    } else if (encoded_char.nb_of_part < 5) {
        decoded_char = decode_morse_letter(encoded_char);
    } else {
        decoded_char = '?';
    }

    return decoded_char;
}


static char decode_morse_letter(encoded_char_t encoded_char) {
    char decoded_char = '?';
    const uint8_t nb_of_letter_indexes[4] = {
        NB_OF_1_PART_LETTERS,
        NB_OF_2_PART_LETTERS,
        NB_OF_3_PART_LETTERS,
        NB_OF_4_PART_LETTERS,
    };
	const uint8_t* const letter_indexes_array[4] = {
        ENCODED_1_PART_LETTERS,
        ENCODED_2_PART_LETTERS,
        ENCODED_3_PART_LETTERS,
        ENCODED_4_PART_LETTERS
    };
    uint8_t index = encoded_char.nb_of_part - 1;
    uint8_t nb_of_indexes = nb_of_letter_indexes[index];

    for (uint8_t i = 0; i < nb_of_indexes; i++) {
		uint8_t letter_index = (letter_indexes_array[index][i] - 'A');
        if (encoded_char_are_equal(&encoded_char, &ENCODED_LETTERS[letter_index]))
            decoded_char = letter_indexes_array[index][i];
    }

    return decoded_char;
}


static char decode_morse_number(encoded_char_t encoded_char) {
    char decoded_char = '?';

    for (uint8_t i = 0; i < NB_OF_ENCODED_NUMBERS; i++) {
        if (encoded_char_are_equal(&encoded_char, &ENCODED_NUMBERS[i]))
            decoded_char = '0' + i;
    }

    return decoded_char;
}


static void decode_signal(morse_decoder_t* this) {
    if (this->additional_character != 0) {
        this->decoded_character = this->additional_character;
        this->additional_character = 0;
    } else {
        if (this->recording_index != this->decoding_index) {
            if (is_decoding_a_character(this)) {
                if (is_receiving_an_off_signal(this) &&
                (this->signal_duration_records[this->recording_index] > (2 * this->reference_duration)))
                    decode_character(this);
            } else {
                decode_blank(this);
            }
        }
    }
}