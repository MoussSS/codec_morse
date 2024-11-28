#include "util.h"


const encoded_char_t ENCODED_NUMBERS[NB_OF_ENCODED_NUMBERS] = {
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

const encoded_char_t ENCODED_LETTERS[NB_OF_ENCODED_LETTERS] = {
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

const uint8_t ENCODED_1_PART_LETTERS[NB_OF_1_PART_LETTERS] = {'E', 'T'};
const uint8_t ENCODED_2_PART_LETTERS[NB_OF_2_PART_LETTERS] = {'A', 'I', 'M', 'N'};
const uint8_t ENCODED_3_PART_LETTERS[NB_OF_3_PART_LETTERS] = {'D', 'G', 'K', 'O', 'R', 'S', 'U', 'W'};
const uint8_t ENCODED_4_PART_LETTERS[NB_OF_4_PART_LETTERS] = {'B', 'C', 'F', 'H', 'J', 'L', 'P', 'Q', 'V', 'X', 'Y', 'Z'};


bool encoded_char_are_equal(const encoded_char_t* char1, const encoded_char_t* char2) {
    bool char_are_equals = (char1->nb_of_part == char2->nb_of_part);

    if (char_are_equals) {
        for (uint8_t i = 0; i < char1->nb_of_part; i++)
            char_are_equals &= (char1->char_parts[i] == char2->char_parts[i]);
	}

    return char_are_equals;
}


uint8_t increment_circular_index(uint8_t index, uint8_t size) {
    uint8_t incremented_index = index + 1;
    if (incremented_index >= size) {
        incremented_index = 0;
    }
    return incremented_index;
}
