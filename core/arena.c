#include "arena.h"
#include "types.h"

void arena_init(Arena* arena, void* p_mem, size_t size) {
	if (!arena) {
		printf("Arena is NULL!");
	}

	arena->p_mem = p_mem;
	arena->size = size;
	arena->pos = 0;

}

void arena_display_info(Arena* p_arena) {
	printf(
		"\nMemory Info\nSize: %zu bytes\nMemory Left %zu bytes\n",
		p_arena->size, p_arena->size - p_arena->pos
	);
}

void* arena_push(Arena *arena, size_t size) {
	if (!arena) {
		return NULL;
	}

	void* ret_ptr = NULL;
	size_t old_pos = arena->pos;

	if (old_pos % size != 0) {
		old_pos = (old_pos + size - 1)/size * size;	
	}


	size_t new_pos = old_pos + size;
	

	if (new_pos <= arena->size) {
		ret_ptr = (byte*)arena->p_mem + arena->pos;
		arena->pos = new_pos;
	}
	else {
		printf("Ran out of memory");
	}


	return ret_ptr;	
}
