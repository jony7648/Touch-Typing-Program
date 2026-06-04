#include "gc_widget.h"
#include "core/slice.h"

typedef struct {
	GCWidgetDefCommon common;
	GCWidgetDefUnique unique;
} GCScrollWindowDef;

typedef struct {
	GCWidget gc_widget;
	GCWidgetSlice children_slice;
} GCScrollWindow;

GCScrollWindow *gc_scroll_window_create(GCScrollWindowDef def); 
void gc_scroll_window_present(GCScrollWindow *scroll_win); 
