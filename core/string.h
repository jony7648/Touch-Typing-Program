
#pragma once
#include "arena.h"
#include <stdio.h>

typedef struct {
	size_t len;
	size_t max_len;
	char *p_mem;
} String;


String* StringCreate(Arena* arena, size_t max_len);
String* StringCpy(Arena* arena, String *src_str, size_t new_str_len);
void StringSet(String* str, char *c_str);
void StringAppend(String* str, char *c_str);
void StringDisplay(String* str);
void StringDisplayInfo(String* str);
void StringToUpper(String* str);
void StringToLower(String* str);
const char* StringToCstr(Arena* p_arena, String* str);

