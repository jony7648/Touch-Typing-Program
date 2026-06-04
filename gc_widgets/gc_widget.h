#pragma once
#include "core/error.h"
#include "core/listener.h"


#include "gc_widgets/gc_types.h"
#include "core/space.h"

#include <stdio.h>

#include <gtk/gtk.h>

typedef struct GCWidget GCWidget;

typedef struct {
	short tag;
	gboolean hexpand;
	gboolean vexpand;
	GtkAlign halign;
	GtkAlign valign;
} GCWidgetDefCommon;



typedef struct {
	Rect rect;
} GCWidgetDefUnique;


//ensure that you include this member as the first when creating a gc widget,
//as it allows for the casting of a GtkWidget* to any GCWidget pointer
struct GCWidget {
	Rect rect;
	GtkWidget* g_widget;
	Listener *listener;
	GCType widget_type;
	short tag;
	gboolean is_presenting;

	//takes a destructor_ptr that returns the malloced memory, so it can be freed
	//ensure that the actuall type that contains this struct is returned
	void *(*destructor_ptr)(GCWidget*);
};

void gc_widget_display_info(GCWidget *widget);
void gc_widget_init(GCWidget* widget, GtkWidget* gtk_widget, void *(*destructor_ptr)(GCWidget*), GCType type, GCWidgetDefCommon com, GCWidgetDefUnique unique);
GCError gc_widget_free(GCWidget *gc_widget);

