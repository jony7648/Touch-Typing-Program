#pragma once
#include "gc_widget.h"

typedef struct {
	GCWidgetDefCommon common;
	GCWidgetDefUnique unique;
	char *init_text; 
	bool editable;
	GtkWrapMode wrap_mode;
} GCTextViewDef;

typedef struct {
	GCWidget gc_widget;
} GCTextView;

GCTextView *gc_text_view_create(GCTextViewDef def);
void gc_text_view_set_text(GCTextView *text_view, const char *text);
