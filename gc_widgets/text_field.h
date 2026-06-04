#pragma once
#include "gc_widget.h"

#include "core/listener.h"

typedef struct {
	GCWidgetDefCommon common;
	GCWidgetDefUnique unique;
	char *initial_text;
} GCTextFieldDef;

typedef struct {
	GCWidget gc_widget;
	Listener listener;
} GCTextField;

GCTextField *gc_text_field_create(GCTextFieldDef def);

