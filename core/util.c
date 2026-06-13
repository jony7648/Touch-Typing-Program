#include "util.h"
#include "str_view.h"

#include <string.h>
#include <stdlib.h>
#include <gtk/gtk.h>


uint8_t util_get_char_size(unsigned char c) {
	uint8_t c_size2 = 0xC3;
	uint8_t r_size2 = 195;

	if ((c & c_size2) == r_size2) {return 2;};

	return 1;
}

char *util_append_c_strs(char **c_str_arr, int count) {
	size_t combine_len = 0;
	
	for (int i=0; i<count; i++) {
		combine_len += strlen(c_str_arr[i]);
	}

	

	char *new_str = malloc(combine_len + 1);	
	new_str[0] = '\0';


	for (int i=0; i<count; i++) {
		strcat(new_str, c_str_arr[i]);
	}

	return new_str;
}





unsigned char util_rotate_byte_left(unsigned char b, unsigned char c) {
	c %= 8;

	unsigned char res = b;
	for (int i = 0; i<c; i++) {
		res = (res << 1) | (res >> 7); 
	}

	return res;
}
unsigned char util_rotate_byte_right(unsigned char b, unsigned char c) {
	c %= 8;

	unsigned char res = b;
	for (int i = 0; i<c; i++) {
		res = (res >> 1) | (res << 7); 
	}

	return res;
}

void util_display_bytes(unsigned int val) {
	int iter = 0;

	for (int i=0; i<8; i++) {
		int bit = (val & 128 ) ? 1 : 0;
	 
		printf("%d", bit);

		val = util_rotate_byte_left(val, 1);

	}

	puts("");

}

GMenuModel *util_get_submenu_from_model(GMenuModel *model, char *target_menu_name) {
	const char *label = NULL;


	if (!model) {
		return NULL;
	}

	int count = g_menu_model_get_n_items(model);
	int item_index = 0;


	StrView view;

	 for (int i=0; i<count; i++) {
		g_menu_model_get_item_attribute(model,i,G_MENU_ATTRIBUTE_LABEL,"s", &label);

		str_view_init(&view, label);

		int found = str_view_cmp(&view, target_menu_name);

		if (found == 0) {
			item_index = i;
		}
	 }

	GMenuModel *sub_model = g_menu_model_get_item_link(model, 0, G_MENU_LINK_SUBMENU);

	return sub_model;
}
