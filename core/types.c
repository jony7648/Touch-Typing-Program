#include "types.h"

void ichar_display(IChar ichar) {
	for (int i=0; i<ichar.size; i++) {
		printf("%c", ichar.data[i]);
	}

}
