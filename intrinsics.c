#include "intrinsics.h"


void memset(void* dest, unsigned char value, unsigned long int size) {
    unsigned char* ptr = (unsigned char*) dest;
    while ((size--)) {
        *ptr = value;
        ++(ptr);
    }
}

void* memcpy(void* dest, const void* src, unsigned long int size) {
    unsigned char* ptr_src = (unsigned char*) src;
    unsigned char* ptr_dest = (unsigned char*) dest;
    while ((size--)) {
        *ptr_dest = *ptr_src;
        ++(ptr_src);
        ++(ptr_dest);
    }

    return dest;
}

unsigned long int strlen(const char* s) {
    unsigned long int count = 0;
    while (*s) {
        ++count;
        ++(s);
    }

    return count;
}
