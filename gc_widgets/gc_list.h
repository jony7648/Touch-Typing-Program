#include "gc_widget.h"

typedef struct {
	GCWidgetDefCommon common;
	GCWidgetDefUnique unique;
	const char **string_arr;
} GCListDef;

typedef struct {
	GCWidget gc_widget;
	GtkStringList *string_list;
} GCList;

GCList *gc_list_create(GCListDef def);
