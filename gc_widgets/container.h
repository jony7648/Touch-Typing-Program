#pragma once

#include "core/slice.h"

#include <stdio.h>

#include "gc_widget.h"


typedef struct {
	GCWidgetDefCommon common;
	GCWidgetDefUnique unique;
	Point spacing;
} GCContainerDef;

typedef struct {
	GCWidget gc_widget;
	GCWidgetSlice widget_slice;
} GCContainer;


typedef struct {
	GCContainer *container;
	GCType widget_type;
	int signal_id;
	int tag;
	void *signal_data;
} GCContainerSignalTagCallParameters;

void gc_container_init(GCContainer *p, GCContainerDef *def);
GCContainer *gc_container_create(GCContainerDef def);
GCError gc_container_attach(GCContainer *p, GCWidget *widget);
GCError gc_container_display(GCContainer *p);
void gc_container_signal_tag_call(GCContainerSignalTagCallParameters* parameters);
