#include "util.h"


uint8_t increment_circular_index(uint8_t index, uint8_t size) {
    uint8_t incremented_index = index + 1;
    if (incremented_index >= size) {
        incremented_index = 0;
    }
    return incremented_index;
}
