#include <stdint.h>
#include <string.h>

void* memcpy(void* dst, const void* src, size_t size) {
    int i = 0;
    while (size > 0) {
        ((uint8_t*) dst)[i] = ((uint8_t*) src)[i];
        ++i;
        --size;
    }

    return dst;
}

