#include "arena.h"
#include "string.h"

#include <stdint.h>
#include <math.h>
#include <string.h>

#define LOWER_LETTER_START 97
#define UPPER_LETTER_START 65
#define LETTER_COUNT 26

String* StringCreate(Arena* arena, size_t max_len) {
	size_t alloc_ammount = sizeof(String) + (max_len * sizeof(char));


	String* str;

	str = arena_push(arena, alloc_ammount);	

	if (!str) {
		printf("Fail no mem left\n");
		return NULL;
	}

	str->len = 0;
	str->max_len = max_len;
	str->p_mem = (char*)(str + 1);

	printf("\n%p p_mem\n", str->p_mem);
	 
	return str;
}

String* StringCpy(Arena* arena, String* src_str, size_t new_str_len) {
	String* new_str = StringCreate(arena, new_str_len);

	if (!new_str) {
		return NULL;
	}

	int i = 0;

	for (i=0; i<new_str_len; i++) {
		if (i >= src_str->len) {
			break;
		}

		new_str->p_mem[i] = src_str->p_mem[i];
	}


	new_str->len = i;
	return new_str;
}

void StringSet(String* str, char* c_str) {
	int i = 0;

	str->len = 0;


	for (size_t i=0; c_str[i] != '\0'; i++) {
		if (str->len >= str->max_len) {
			break;
		}

		str->p_mem[str->len] = c_str[i];
		str->len = str->len + 1;
	}
}

void StringAppend(String* str, char* c_str) {
	if (!str) {
		return;
	}
}

void StringDisplay(String* str) {
	for (int i=0; i<str->len; i++) {
		printf("%c", str->p_mem[i]);
	}

	printf("\n");
}

void StringDisplayInfo(String* str) {
	puts("\nString Info");
	printf("Len: %zu \nMaxLen: %zu \nString Contents: ", str->len, str->max_len);

	for (int i=0; i<str->len; i++) {
		printf("%c", str->p_mem[i]);
	}
	
	printf("\n\n");
}


void StringToUpper(String* str) {
	int adjust_count = UPPER_LETTER_START - LOWER_LETTER_START;
	int end_bound = LOWER_LETTER_START + LETTER_COUNT;


	for (int i=0; i<str->len; i++) {
		if (str->p_mem[i] >= LOWER_LETTER_START && i <= end_bound) {
			str->p_mem[i] += adjust_count;
		}
	}
}

void StringToLower(String* str) {
	int adjust_count = LOWER_LETTER_START - UPPER_LETTER_START;
	int end_bound = UPPER_LETTER_START + LETTER_COUNT;


	for (int i=0; i<str->len; i++) {
		if (str->p_mem[i] >= UPPER_LETTER_START && i <= end_bound) {
			str->p_mem[i] += adjust_count;
		}
	}
}

const char* StringToCstr(Arena* p_arena, String* str) {
	char* ret_ptr = arena_push(p_arena, sizeof(char) * str->len + 1);

	if (!ret_ptr) {
		return NULL;
	}

	int i = 0;

	for (i=0; i<str->len; i++) {
		ret_ptr[i] = str->p_mem[i];
	}

	//add null terminator
	ret_ptr[i] = '\0';
	

	return ret_ptr;
}


