#pragma once
#include "space.h"


#include <gtk/gtk.h>

typedef struct {
	Point dimensions;
	char* title;	
	int scene_id;
	GtkWidget* g_widget;
} GCWin;

GCWin *gc_win_create(GtkApplication *app, char *title, Point dimensions);
void gc_win_display(GCWin* win);
