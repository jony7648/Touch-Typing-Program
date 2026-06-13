#pragma once

#include <stdio.h>
#include <stdint.h>
#include <gtk/gtk.h>

uint8_t util_get_char_size(unsigned char c);
char *util_append_c_strs(char **c_str_arr, int count);
unsigned char util_rotate_byte_left(unsigned char b, unsigned char c); 
unsigned char util_rotate_byte_right(unsigned char b, unsigned char c); 
void util_display_bytes(unsigned int val);
GMenuModel *util_get_submenu_from_model(GMenuModel *model, char *target_menu_name); 

