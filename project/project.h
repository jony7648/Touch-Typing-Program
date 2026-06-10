#pragma once

#include "core/slice.h"
#include "core/app.h"
#include "core/time_componet.h"
#include "core/str_view.h"
#include "paths.h"

enum WidgetTags {
	TagTypingTextField,
};



typedef struct {
	Slice *p_layout_slice;
	GCApp *p_app;
	GCTime *p_time;
} LaunchData;
