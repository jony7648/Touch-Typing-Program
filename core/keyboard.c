#include "keyboard.h"
#include "stdio.h"
#include "util.h"


/*
PhysicalChar MacKeyCodes[] = {
	{12,'Q'},
	{13,'W'},
	{14,'E'},
	{15,'R'},
	{17,'T'},
	{16,'Y'},
	{32,'U'},
	{34,'I'},
	{31,'O'},
	{35,'P'},
	
	{0,'A'},
	{1,'S'},
	{2,'D'},
	{3,'F'},
	{5,'G'},
	{4,'H'},
	{38,'J'},
	{40, 'K'},
	{37, 'L'},
	
	{6, 'Z'},
	{7, 'X'},
	{8, 'C'},
	{9, 'V'},
	{11, 'B'},
	{45, 'N'},
	{46, 'M'},
};




gboolean signal_on_key_pressed(GtkWidget* widget, GtkEventControllerKey *controller, guint keycode, guint keyval, GdkModifierType state, gpointer user_data) { 
	gunichar c = gdk_keyval_to_unicode(keyval);

	if (c != 0) {
		char utf8[7];
		int len = g_unichar_to_utf8(c, utf8);
		utf8[len] = '\0';

		//g_print("Char: %s\n", utf8);
	}

	g_print("%c \n", keyboard_get_mac_key(keycode));

	return FALSE;
}

char keyboard_get_mac_key(unsigned short code) {
	char ret_char = '0';
	int len = sizeof(MacKeyCodes) / sizeof(MacKeyCodes[0]);
	
	for (int i=0; i<len; i++) {
		if (code == MacKeyCodes[i].code) {
			ret_char = MacKeyCodes[i].key;
			break;
		}
	}
	
	return ret_char;
}
*/

void keyboard_layout_init(KeyboardLayout *layout, char *name, char key_arr[KEYBOARD_KEY_COUNT]) {
}

GCError keyboard_layout_load(KeyboardLayout *layout, char *path) {
	FILE *fptr = fopen(path, "r");

	if (!fptr) {
		return ErrorFileAccessFailed;
	}

	char buff[100];
	short buff_size = sizeof(buff);

	fgets(buff, buff_size, fptr);



	int key_count = -1;

	while(fgets(buff, buff_size, fptr)) {
		key_count++;

		if (key_count >= KEYBOARD_KEY_COUNT) {
			break;
		}


		IChar *p_ichar = &layout->keys[key_count];


		char first_byte = buff[0];		

		p_ichar->size = get_char_size(first_byte);
	

		//fill char data
		for (int i=0; i<p_ichar->size; i++) {
			p_ichar->data[i] = buff[i];
		}	


		p_ichar->data[p_ichar->size] = '\0';
	}


	fclose(fptr);
	fptr = NULL;
	return ErrorClear;
}



void keyboard_layout_display_info(KeyboardLayout *keyboard_layout) {
	for (int i=0; i<KEYBOARD_KEY_COUNT; i++) {
		ichar_display(keyboard_layout->keys[i]);
	}
		puts("");
}


