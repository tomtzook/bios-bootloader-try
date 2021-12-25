#pragma once

#include <types.h>


static inline void revmemcpy(char* dest, const char* src, size_t len) {
    src = src + len - 1;
    while (len-- > 0) {
        *dest = *src;
        --(src);
        ++(dest);
    }
}

static inline void itoa(char* buffer, int value) {
    char fmtbuffer[20];

    size_t index = 0;
    do {
        int n = value % 10;
        char digit = static_cast<char>(n);
        fmtbuffer[index++] = '0' + digit;
        value = value / 10;
    } while (value);

    revmemcpy(buffer, fmtbuffer, index);
}
