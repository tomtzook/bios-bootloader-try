
#include <x86/cpuid.h>
#include <x86/msr.h>
#include <x86/cr.h>

#include "screen.h"
#include "printf.h"

#define CPUID_VENDOR_INTEL "GenuineIntel"

static bool check_vendor_id_intel() {
    char buffer[12 + 1] = {0};
    auto cpuid = x86::cpuid<0x0>();
    memcpy(buffer, &cpuid.ebx, 4);
    memcpy(buffer + 4, &cpuid.edx, 4);
    memcpy(buffer + 8, &cpuid.ecx, 4);

    return 0 == strcmp(buffer, CPUID_VENDOR_INTEL);
}

extern "C"
void main() {
    vga::screen_clear();
    vga::screen_print("Hello World\nyep");

    char buf[20] = {0};
    itoa(buf, 20);
    vga::screen_print(buf);

    if (!check_vendor_id_intel()) {
        vga::screen_print("Model is not intel\n");
        return;
    }

    while (true);
}
