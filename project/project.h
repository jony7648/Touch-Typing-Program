#pragma once

#include "core/slice.h"
#include "core/app.h"
#include "core/str_view.h"

enum WidgetTags {
	TagTypingTextField,
};



typedef struct {
	Slice *p_layout_slice;
	GCApp *p_app;
} LaunchData;
