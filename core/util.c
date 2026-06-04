#include "util.h"

uint8_t get_char_size(unsigned char c) {
	uint8_t c_size2 = 0xC3;
	uint8_t r_size2 = 195;

	if ((c & c_size2) == r_size2) {return 2;};

	return 1;
}

