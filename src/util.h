#ifndef _UTIL_H_
#define _UTIL_H_

#include <stdbool.h>
#include <stdint.h>

// TODO move in a common.h file
#define TIME_TO_CYCLE_FACTOR 10
#define MORSE_REFERENCE_DURATION_MS 150
#define NB_OF_ENCODED_NUMBERS 10
#define NB_OF_ENCODED_LETTERS 26

#define MAX_PART_IN_CHARACTER 5

typedef enum {
    DOT  = 0,
    DASH = 1
} morse_part_e;

typedef struct encoded_char_s {
    uint8_t      nb_of_part;
    morse_part_e char_parts[MAX_PART_IN_CHARACTER];
} encoded_char_t;

extern const encoded_char_t ENCODED_NUMBERS[NB_OF_ENCODED_NUMBERS];
extern const encoded_char_t ENCODED_LETTERS[NB_OF_ENCODED_LETTERS];

#define NB_OF_1_PART_LETTERS 2
#define NB_OF_2_PART_LETTERS 4
#define NB_OF_3_PART_LETTERS 8
#define NB_OF_4_PART_LETTERS 12

extern const uint8_t ENCODED_1_PART_LETTERS[NB_OF_1_PART_LETTERS];
extern const uint8_t ENCODED_2_PART_LETTERS[NB_OF_2_PART_LETTERS];
extern const uint8_t ENCODED_3_PART_LETTERS[NB_OF_3_PART_LETTERS];
extern const uint8_t ENCODED_4_PART_LETTERS[NB_OF_4_PART_LETTERS];

// end TODO

bool encoded_char_are_equal(const encoded_char_t* char1, const encoded_char_t* char2);
uint8_t increment_circular_index(uint8_t index, uint8_t size);


#endif //_UTIL_H_