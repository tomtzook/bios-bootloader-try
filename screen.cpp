#include <x86/io.h>
#include <intrinsics.h>
#include "screen.h"

#define VGA_CTRL_REGISTER 0x3d4
#define VGA_DATA_REGISTER 0x3d5
#define VGA_OFFSET_LOW 0x0f
#define VGA_OFFSET_HIGH 0x0e

#define VIDEO_ADDRESS 0xb8000
#define WHITE_ON_BLACK 0x0f


namespace vga {

static pos_t get_cursor() {
    x86::outb(VGA_CTRL_REGISTER, VGA_OFFSET_LOW);
    size_t offset = x86::inb(VGA_DATA_REGISTER);
    x86::outb(VGA_CTRL_REGISTER, VGA_OFFSET_HIGH);
    offset |= (x86::inb(VGA_DATA_REGISTER) << 8);
    return { offset };
}

static void set_cursor(pos_t pos) {
    size_t offset = pos.offset();
    x86::outb(VGA_CTRL_REGISTER, VGA_OFFSET_LOW);
    x86::outb(VGA_DATA_REGISTER, static_cast<uint8_t>(offset & 0xff));
    x86::outb(VGA_CTRL_REGISTER, VGA_OFFSET_HIGH);
    x86::outb(VGA_DATA_REGISTER, static_cast<uint8_t>((offset >> 8) & 0xff));
}

static void screen_put(char character, pos_t pos) {
    size_t offset = pos.offset() * 2;
    auto* mem = reinterpret_cast<unsigned char*>(VIDEO_ADDRESS);
    mem[offset] = character;
    mem[offset + 1] = WHITE_ON_BLACK;
}

void screen_clear() {
    for (size_t i = 0; i < SCREEN_WIDTH * SCREEN_HEIGHT; ++i) {
        screen_put(' ', {i});
    }
    set_cursor({0, 0});
}

pos_t screen_put(char character) {
    auto pos = get_cursor();
    if (character == '\n') {
        pos.y += 1;
        pos.x = 0;
    } else {
        screen_put(character, pos);
        pos.x += 1;
    }

    set_cursor(pos);
    return pos;
}

pos_t screen_print(const char* str) {
    auto pos = get_cursor();
    while (*str) {
        if (*str == '\n') {
            pos.y += 1;
            pos.x = 0;
        } else {
            screen_put(*str, pos);
            pos.x += 1;
        }
        ++(str);
    }

    set_cursor(pos);
    return pos;
}

}
