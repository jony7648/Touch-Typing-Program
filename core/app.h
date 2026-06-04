#pragma once
#include "space.h"
#include "window.h"
#include "core/scene.h"
#include <gtk/gtk.h>

typedef struct {
	GCWin **p_mem;
	short size;
} WinArr;

typedef struct {
	GCScene **p_mem;
	short size;
} SceneArr;

typedef struct {
	const char *name;
	const char *app_id;
	Point dimensions;
	GtkApplication *g_component;
	WinArr win_arr;
	SceneArr scene_arr;
	int argc;
	int status;
	char **argv;
} GCApp;

void gc_app_init(GCApp *p_app, int argc, char **argv);
void gc_app_create_wins(GCApp *p, unsigned int win_count);
void gc_app_run(GCApp *p_app, void(*activate_func)(GtkApplication*, gpointer), void* p_signal);
GCError gc_app_attach_scenes(GCApp *p, GCScene **scene_arr, int scene_count);
GCError gc_app_display_scene(GCApp *p, int scene_index);
