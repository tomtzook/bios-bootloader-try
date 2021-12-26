#pragma once

#include <x86/paging/paging.h>
#include <x86/paging/bit32.h>

#pragma pack(push, 1)

#define page_aligned __attribute__ ((aligned(x86::paging::page_size_4k)))

struct page_table_t {
    // mapping only a single dir = 4096 MB
    // using big pdes
    x86::paging::bit32::pde_t pdes[x86::paging::bit32::pdes_in_directory] page_aligned;
} page_aligned;

void setup_identity_page_table(page_table_t& page_table) noexcept;

#pragma pack(pop)
