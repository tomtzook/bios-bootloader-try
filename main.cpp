
#include <x86/cpuid.h>
#include <x86/msr.h>
#include <x86/cr.h>
#include <x86/paging/paging.h>
#include <x86/paging/bit32.h>

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

static void print_paging_info() {
    auto mode = x86::paging::current_mode();
    switch (mode) {
        case x86::paging::mode_t::DISABLED:
            vga::screen_print("Paging: Disabled\n");
            break;
        case x86::paging::mode_t::BIT32:
            vga::screen_print("Paging: 32 BIT mode\n");
            break;
        case x86::paging::mode_t::PAE:
            vga::screen_print("Paging: PAE mode\n");
            break;
        case x86::paging::mode_t::IA32E:
            vga::screen_print("Paging: IA32E mode\n");
            break;
    }

    auto maxphysaddr = x86::paging::max_physical_address_width();
    vga::screen_print("Max Physical Address Width: ");
    vga::screen_print(maxphysaddr);
    vga::screen_print("\n");

    if (x86::paging::bit32::is_pse36_supported()) {
        vga::screen_print("PSE36 Supported\n");
    } else {
        vga::screen_print("PSE36 Not Supported\n");
    }

    if (x86::paging::bit32::are_4m_page_tables_supported()) {
        vga::screen_print("32 BIT 4M Pages Supported\n");
    } else {
        vga::screen_print("32 BIT 4M Pages Not Supported\n");
    }

    // demonstrating translation in 32bit paging
    /*x86::paging::bit32::linear_address_t addr{};
    addr.raw = 0x1000;

    auto cr3 = x86::read<x86::cr3_t>();
    auto pde_address = (static_cast<physical_address_t>(cr3.bits.page_table_address) << x86::paging::page_bits_4k) |
            (addr.big.directory << 2);
    auto pde = reinterpret_cast<const x86::paging::bit32::pde_t*>(pde_address);
    if (pde->is_big()) {
        auto physical_address = pde->address() | (addr.big.offset);
    } else {
        auto pte_address = pde->address() | (addr.small.table << 2);
        auto pte = reinterpret_cast<const x86::paging::bit32::pte_t*>(pte_address);
        auto physical_address = pte->address() | (addr.small.offset);
    }*/
}

extern "C"
void main() {
    vga::screen_clear();
    vga::screen_print("Hello World\n");

    if (!check_vendor_id_intel()) {
        vga::screen_print("Model is not intel\n");
        return;
    }

    print_paging_info();

    while (true);
}