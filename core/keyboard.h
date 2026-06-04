#pragma once

#include "types.h"
#include "heap_str.h"
#include "core/error.h"

#define KEYBOARD_LAYOUT_EXTENSION ".layout"
#define KEYBOARD_ROWS 3
#define KEYBOARD_KEYS_PER_ROW 10
#define KEYBOARD_KEY_COUNT KEYBOARD_ROWS * KEYBOARD_KEYS_PER_ROW

typedef struct {
	unsigned short code;
	char key;
} PhysicalChar;

typedef struct {
	char *name;
	IChar keys[KEYBOARD_KEY_COUNT];
} KeyboardLayout;

char keyboard_get_mac_key(unsigned short code);
GCError keyboard_layout_load(KeyboardLayout *layout, char *path);
void keyboard_layout_display_info(KeyboardLayout *keyboard_layout);

