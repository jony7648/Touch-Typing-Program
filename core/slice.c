#include "slice.h"
#include "types.h"

void GCSliceDisplayInfo(GCWidgetSlice *p) {
	puts("\nSlice Info");
	g_print("Size: %zu\n Max Size: %zu\n", p->count, p->max_count);	
}

void SliceInit(Slice *p_slice, size_t type_size, size_t count) {
	p_slice->max_size = sizeof(char) * (count * type_size);
	p_slice->type_size = type_size;
	p_slice->p_mem = malloc(p_slice->max_size);
	p_slice->count = 0;
}

void *SliceRequest(Slice *p) {
	size_t new_count = (p->count+1);
	size_t new_size = new_count * p->type_size;

	if (new_size > p->max_size) {
		p->p_mem = realloc(p->p_mem, p->max_size*2);
	}

	p->count++;

	char *ptr = (char*)p->p_mem;
	ptr += (p->count - 1) * p->type_size;

	return ptr;
}

void SliceFree(Slice *p) {
	free(p->p_mem);
	p->p_mem = NULL;
	p->count = 0;
}

/*
void IntSliceAppend(Slice *p_slice, int value) {
	size_t new_size = (p_slice->count+1)*p_slice->slice_size;

	if (new_size > p_slice->max_size) {
		p_slice->p_mem = realloc(p_slice->p_mem, p_slice->max_size*2);
	}

	int* ptr = p_slice->p_mem;

	ptr[p_slice->count] = value;

	p_slice->count++;
}
*/

void GCWidgetSliceInit(GCWidgetSlice *p_slice, size_t count) {
	p_slice->max_count = count;
	p_slice->p_mem = malloc(count * sizeof(GCWidget*));
	p_slice->count = 0;
}

void GCWidgetSliceAppend(GCWidgetSlice *p_slice, GCWidget *g_widget) {
	size_t new_count = p_slice->count + 1;

	if (new_count > p_slice->max_count) {
		size_t new_ammount = p_slice->max_count*2;
		size_t new_size = new_ammount * sizeof(GCWidget*);
		p_slice->p_mem = realloc(p_slice->p_mem, new_size);

		p_slice->max_count = new_ammount;
	}

	p_slice->p_mem[p_slice->count] = g_widget;
	p_slice->count++;
}
