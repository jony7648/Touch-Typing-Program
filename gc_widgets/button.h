#pragma once
#include "gc_widget.h"

typedef struct {
	GCWidgetDefCommon common;
	GCWidgetDefUnique unique;
	const char* text;
} GCButtonDef;

typedef struct {
	GCWidget gc_widget;
} GCButton;

GCButton* gc_button_create(GCButtonDef def);


