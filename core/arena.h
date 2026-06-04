#pragma once
#include <stdio.h>

typedef struct {
	size_t size;
	size_t pos;
	void* p_mem;
} Arena;

void arena_init(Arena* arena, void *p_mem, size_t size);
void *arena_push(Arena *arena, size_t size);
void arena_display_info(Arena* p_arena);

