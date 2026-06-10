#pragma once

#include "arena.h"
#include "error.h"

#include <stdio.h>

typedef struct {
	const char *ptr;
	size_t len;
} StrView;

typedef struct {
	StrView view;
	const StrView *src_view;
	size_t pos;
} StrViewIterator;

void str_view_init(StrView *str_view, const char *c_str); 
void str_view_slice(StrView *str_view, size_t start, size_t end); 
void str_view_display(StrView *str_view); 
void str_view_display_info(StrView *str_view); 
size_t str_view_split(StrView *str, Arena *arena, char split_char, bool remove_empty);
void str_view_strip(StrView *str_view); 
void str_view_strip_extension(StrView *str_view);
void str_view_get_extension(StrView *str_view);
bool str_view_starts_with(StrView *str_view, char start_char);
bool str_view_ends_with(StrView *str_view, char end_char);
int str_view_cmp(StrView *str_view, const char *c_str);
char *str_view_to_cstr(StrView *str_view, Arena *p_arena);
char *str_view_to_heap_cstr(StrView *str_view); 
GCError str_view_to_buffer(StrView *p, char *buff, size_t buff_size);
int str_view_to_int(StrView *view);

void str_view_iterator_init(StrViewIterator *it, const StrView *src_view);
bool str_view_iterator_has_next(StrViewIterator *it);
