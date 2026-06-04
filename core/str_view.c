#include "str_view.h"
#include "arena.h"

#include <stdlib.h>
#include <string.h>

void str_view_init(StrView *p, char *c_str) {
	p->len = 0;
	p->ptr = c_str;

	int len = 0;

	//increment len until we hit a nullterminator
	for (len=0; c_str[len] != '\0'; len++);

	p->len = len;
	
}

void str_view_slice(StrView *str, size_t start, size_t end) {
	str->ptr += start;
	str->len = end - start;	
}

void str_view_display(StrView *str) {
	for (size_t i=0; i<str->len; i++) {
		printf("%c", str->ptr[i]);
	}

	puts("");
}

void str_view_display_info(StrView *str) {
	puts("\nStrView Info");
	printf("Str: ");
	for (size_t i=0; i<str->len; i++) {
		printf("%c", str->ptr[i]);
	}

	printf("\nLen: %zu\n", str->len);

}

void str_view_strip(StrView *view) {
	size_t start = 0;
	size_t end = view->len;


	for (int i=0; i<view->len; i++) {
		if (view->ptr[i] != ' ' && view->ptr[i] != '\n') {
			start = i;
			break;
		}
	}

	for (int i=view->len-1; i>=0; i--) {
		if (view->ptr[i] == ' ' || view->ptr[i] == '\n') {
			end--;
			continue;
		}
		break;
	}

	view->ptr = view->ptr + start;
	view->len = end - start;
}


void str_view_strip_extension(StrView *p) {
	char target_char = '.';

	for (int i=p->len-1; i>=0; i--) {
		if (p->ptr[i] == target_char) {
			p->len = i;
			break;
		}
	}
}

void str_view_get_extension(StrView *p) {
	char target_char = '.';

	for (int i=p->len-1; i>0; i--) {
		if (p->ptr[i] == target_char) {
			p->ptr += i;
			p->len -= i;
			break;
		}
	}
}

int str_view_cmp(StrView *view, char *c_str) {
	if (strlen(c_str) > view->len) {
		return -1;
	}

	for (int i=0; i<view->len; i++) {
		char c_char = c_str[i];
		char v_char = view->ptr[i];

		//no need to check for \0 as str_views do not have them
		if (v_char != c_char) {
			return i+1;
		}
	}

	return 0;
}

bool str_view_starts_with(StrView *p, char start_char) {
	return p->ptr[0] == start_char; 
}

bool str_view_ends_with(StrView *p, char end_char) {
	return p->ptr[p->len - 1] == end_char; 
}

char *str_view_to_heap_cstr(StrView *str) {
	char *c_str = malloc(sizeof(char) * (str->len + 1));

	if (!c_str) {
		return NULL;
	}
	
	memcpy(c_str, str->ptr, str->len);
	c_str[str->len] = '\0';

	return c_str;
}

char *str_view_to_cstr(StrView *p_view, Arena *p_arena) {
	char *c_str = arena_push(p_arena, (p_view->len+1) * sizeof(char));

	if (!c_str) {
		return NULL;
	}

	memcpy(c_str, p_view->ptr, p_view->len);
	c_str[p_view->len] = '\0';

	return c_str;
}

GCError str_view_to_buffer(StrView *p, char *buff, size_t buff_size) {
	if (buff_size <= p->len) {
		return ErrorInadequateBufferSize;	
	}

	memcpy(buff, p->ptr, p->len);

	buff[p->len] = '\0';

	return ErrorClear;
}
