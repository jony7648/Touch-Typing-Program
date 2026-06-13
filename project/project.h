#pragma once

#include "core/slice.h"
#include "core/app.h"
#include "core/time_componet.h"
#include "core/str_view.h"
#include "core/string.h"
#include "paths.h"

#include "gc_widgets/include_me.h"

enum WidgetTags {
	TagTypingTextField,
};



typedef struct {
	Slice *p_layout_slice;
	GCApp *p_app;
	GCDate *p_date;
} LaunchData;

typedef struct {
	GCApp *p_app;		
} SceneCreationParam;
