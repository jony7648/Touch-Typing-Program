#pragma once
#include "gc_widgets/gc_widget.h"
#include <stdio.h>
#include <gtk/gtk.h>

typedef struct {
	size_t count;
	size_t max_count;
	GCWidget** p_mem;	
} GCWidgetSlice;

typedef struct {
	size_t count;
	size_t max_size;
	size_t type_size;
	void* p_mem;	
} Slice;



void GCSliceDisplayInfo(GCWidgetSlice *p_slice);
void SliceInit(Slice *p_slice, size_t slice_size, size_t count);
void *SliceRequest(Slice *p_slice);
void SliceFree(Slice *p_slice);
void GCWidgetSliceInit(GCWidgetSlice *p_slice, size_t count); 
void GCWidgetSliceAppend(GCWidgetSlice *p_slice, GCWidget *g_widget);

