#include "str_view.h"
#include "arena.h"
#include "math.h"

#include <stdlib.h>
#include <string.h>

void str_view_init(StrView *p, const char *c_str) {
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

size_t str_view_split(StrView *str, Arena *arena, char split_char, bool remove_empty) {
	size_t str_count = 0;
	size_t str_start = 0;
	size_t str_end = 0;

	for (size_t i=0; i<str->len; i++) {
		char curr_char = str->ptr[i];		
		bool hit_end_of_text = i == str->len - 1;
		bool should_split = curr_char == split_char || i == str->len;

		if (should_split || hit_end_of_text) {
			size_t view_len = i - str_start + hit_end_of_text;

			if (remove_empty && view_len == 0) {
				continue;
			}

			StrView *view = arena_push(arena, sizeof(StrView));	

			if (!view) {
				return 0;
			}

			view->ptr = str->ptr+str_start;
			view->len = view_len;
			str_count++;
			str_start = i+1;
			continue;
		}
	}


	return str_count;
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

int str_view_cmp(StrView *view, const char *c_str) {
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

int str_view_to_int(StrView *view) {
	int ret_int = 0;
	int multiplier = 1;

	int start_offset = 0;

	const char *str_ptr = view->ptr;

	if (str_ptr[0] == '-') {
		multiplier = -1;	
		start_offset = 1;
	}

	
	int loop_start = view->len-1;
	for (int i=loop_start; i>=start_offset; i--) {
		char curr_char = view->ptr[i];

		if (curr_char >= '0' && curr_char <= '9') {
			ret_int += (curr_char - '0') * pow(10, loop_start - i);	
		}

		else {
			return 0;
		}
	}

	return ret_int;
}


void str_view_iterator_init(StrViewIterator *it, const StrView *src_view) {
	it->pos = 0;
	it->src_view = src_view;
}

bool str_view_iterator_has_next(StrViewIterator *it) {
	int i=it->pos;
	int start = 0;

	if (i >= it->src_view->len) {
		return false;
	}

	//use a while loop here to make sure that i incremets even if the condition
	//fails
	while (it->src_view->ptr[i] == ' ') {
	// if we find a null terminator then there is no more words
		if (it->src_view->ptr[i] == '\0') {
			return false;
		}

		i++;
	}


	start = i;
	it->view.ptr = &it->src_view->ptr[start];


	for (i=i+1; it->src_view->ptr[i] != ' '; i++) {
	// if we find a null terminator then stop searching for spaces
		if (it->src_view->ptr[i] == '\0') {
			break;
		}
	}


	it->view.len = i - start;
	it->pos = i+1;


	return true;
}
