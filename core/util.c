#include "util.h"

#include <string.h>
#include <stdlib.h>


uint8_t get_char_size(unsigned char c) {
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
