#include "io.h"
#include "intrinsics.h"
#include "screen.h"

#define VGA_CTRL_REGISTER 0x3d4
#define VGA_DATA_REGISTER 0x3d5
#define VGA_OFFSET_LOW 0x0f
#define VGA_OFFSET_HIGH 0x0e

#define VIDEO_ADDRESS 0xb8000
#define MAX_ROWS 25
#define MAX_COLS 80

#define WHITE_ON_BLACK 0x0f


static void set_cursor(int offset) {
    offset /= 2;
    outb(VGA_OFFSET_HIGH, VGA_CTRL_REGISTER);
    outb((unsigned char) (offset >> 8), VGA_DATA_REGISTER);
    outb(VGA_OFFSET_LOW, VGA_CTRL_REGISTER);
    outb((unsigned char) (offset & 0xff), VGA_DATA_REGISTER);
}

static int get_cursor() {
    outb(VGA_OFFSET_HIGH, VGA_CTRL_REGISTER);
    int offset = inb(VGA_DATA_REGISTER) << 8;
    outb(VGA_OFFSET_LOW, VGA_CTRL_REGISTER);
    offset += inb(VGA_DATA_REGISTER);
    return offset * 2;
}

static int get_row_from_offset(int offset) {
    return offset / (2 * MAX_COLS);
}

static int get_offset(int col, int row) {
    return 2 * (row * MAX_COLS + col);
}

static int move_offset_to_new_line(int offset) {
    return get_offset(0, get_row_from_offset(offset) + 1);
}

static void putchar(char character, int offset) {
    unsigned char *vidmem = (unsigned char *) VIDEO_ADDRESS;
    vidmem[offset] = character;
    vidmem[offset + 1] = WHITE_ON_BLACK;
}

static int scroll_ln(int offset) {
    memcpy(
            (char *) (get_offset(0, 1) + VIDEO_ADDRESS),
            (char *) (get_offset(0, 0) + VIDEO_ADDRESS),
            MAX_COLS * (MAX_ROWS - 1) * 2
    );

    for (int col = 0; col < MAX_COLS; col++) {
        putchar(' ', get_offset(col, MAX_ROWS - 1));
    }

    return offset - 2 * MAX_COLS;
}

void screen_clear() {
    for (int i = 0; i < MAX_COLS * MAX_ROWS; ++i) {
        putchar(' ', i * 2);
    }
    set_cursor(get_offset(0, 0));
}

void screen_print(const char* str) {
    int offset = get_cursor();
    int i = 0;
    while (str[i] != 0) {
        if (offset >= MAX_ROWS * MAX_COLS * 2) {
            offset = scroll_ln(offset);
        }
        if (str[i] == '\n') {
            offset = move_offset_to_new_line(offset);
        } else {
            putchar(str[i], offset);
            offset += 2;
        }
        i++;
    }
    set_cursor(offset);
}
