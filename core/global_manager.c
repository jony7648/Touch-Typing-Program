#include "global_manager.h"
#include <stdlib.h>

#define GLOBAL_OBJECT_LIST_START_SIZE 5

typedef struct {
	void **p_mem;
	size_t count;
	size_t size;
} GlobalObjectList;

GlobalObjectList G_object_list;


void global_manager_init(GCApp*) {
	G_object_list.size = GLOBAL_OBJECT_LIST_START_SIZE;
	G_object_list.count = 0;
	G_object_list.p_mem = malloc(G_object_list.size * sizeof(void*));
}

GCSizeTOptional global_mananger_add(void *object) {
	size_t new_count = G_object_list.count + 1;

	if (new_count > G_object_list.size) {
		void *new_mem = realloc(G_object_list.p_mem, sizeof(void*) * G_object_list.size * 2);

		if (!new_mem) {
			return (GCSizeTOptional) {0, false};
		}

		G_object_list.p_mem = new_mem;
	}


	G_object_list.p_mem[G_object_list.count] = object;	
	G_object_list.count = new_count;

	return (GCSizeTOptional) {
		.value = G_object_list.count-1,
		.valid = true,
	};
}
