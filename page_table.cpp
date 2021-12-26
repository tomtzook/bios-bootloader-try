#include <intrinsics.h>

#include "page_table.h"


void setup_identity_page_table(page_table_t& page_table) noexcept {
    memset(&page_table, 0, sizeof(page_table_t));

    for (size_t i = 0; i < ARRAY_SIZE(page_table.pdes); ++i) {
        auto& pde = page_table.pdes[i];
        pde.big.present = 1;
        pde.big.rw = 1;
        pde.big.ps = 1;

        pde.address(i * x86::paging::page_size_4m);
    }
}
