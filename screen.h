#pragma once

#include <types.h>

namespace vga {

static constexpr size_t SCREEN_WIDTH = 80;
static constexpr size_t SCREEN_HEIGHT = 25;

struct pos_t {
    size_t x;
    size_t y;

    pos_t(size_t x, size_t y) : x(x), y(y) {}
    pos_t(size_t offset) : x(offset % SCREEN_WIDTH), y(offset / SCREEN_WIDTH) {}

    size_t offset() const { return y * SCREEN_WIDTH + x; }

    pos_t operator+(size_t other) const { return {offset() + other}; }
};

void screen_clear();
pos_t screen_put(char character);
pos_t screen_print(const char* str);
pos_t screen_print(int value);

}
