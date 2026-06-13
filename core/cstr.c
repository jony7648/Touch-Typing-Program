#include "cstr.h"
#include <stdint.h>
#include <string.h>
#include <math.h>

void cstr_from_float(char buffer[], float num, int decimals) {
	//we can convert this to a string by taking advantage of the formula
	//mod(10^d * n, 10^(c+d-s))/10^(c+d-s-1) and flooring our result
	//where n is our number, d is the amount of numbers that come after
	//the decimal, c is the number of whole numbers we have and s is the
	//current iteration

	int whole_count = 0;	
	int decimal_count = 0;

	int int_num = num;

	uint32_t whole_test = 1;

	while (whole_test < num) {
		whole_test *= 10;
		whole_count++;
	}

	decimal_count = whole_count + decimals;

	int_num = num * pow(10,decimals);

	//in case the null terminator was forgotten
	buffer[0] = '\0';



	char char_buffer[2];
	char_buffer[1] = '\0';
	
	//convert the whole numbers
	for (int i=0; i<whole_count; i++) {
		int place_pos = decimal_count - i;

		int place = (int_num % (int)pow(10, place_pos)) / pow(10, place_pos - 1);
		char_buffer[0] = place + '0';

		strcat(buffer, char_buffer);
	}

	//add the decimal
	char_buffer[0] = '.';
	strcat(buffer, char_buffer);

		
	//add the decimal numbers
	for (int i=whole_count; i<decimal_count; i++) {
		int place_pos = decimal_count - i;

		int place = (int_num % (int)pow(10, place_pos)) / pow(10, place_pos - 1);
		char_buffer[0] = place + '0';

		strcat(buffer, char_buffer);
	}
}

GCError cstr_combine(char *buffer, size_t buffer_size, char *str_arr[]) {
	buffer[0] = '\0';
	size_t buffer_len = 0;

	size_t char_pos = 0;
	
	
	for (int i=0; str_arr[i] != NULL; i++) {
		char *str = str_arr[i];
		char test = *str_arr[i];

		size_t str_len = strlen(str);	
		size_t chars_left = buffer_size - char_pos - 1;

		if (str_len > chars_left) {
			puts("BUFFER NOT LARGE ENOUGH FOR ALL C_STRINGS TO BE COMBINED!!");
			return ErrorInadequateBufferSize;
		}

		strcat(buffer, str);
		char_pos += str_len;
	}

	return ErrorClear;
}
