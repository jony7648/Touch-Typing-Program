#pragma once
#include "space.h"


#include <gtk/gtk.h>
#include "core/scene.h"

typedef struct {
	Point dimensions;
	char *title;	
	GCScene *curr_scene;
	GtkWidget *g_container;
	int scene_id;
	GtkWidget* g_widget;
} GCWin;

GCWin *gc_win_create(GtkApplication *app, char *title, Point dimensions);
void gc_win_set_scene(GCWin *p_win, GCScene *scene);
void gc_win_display(GCWin* win);
