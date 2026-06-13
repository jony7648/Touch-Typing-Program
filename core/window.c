#include "app.h"
#include "window.h"
#include "keyboard.h"

#include <stdlib.h>




GCWin *gc_win_create(GtkApplication* g_app, char *title, Point dimensions) {
	GCWin *p_win = malloc(sizeof(GCWin));
	p_win->title = title;
	p_win->scene_id = -1;
	p_win->dimensions = dimensions;
	p_win->curr_scene = NULL;
	p_win->g_container = gtk_grid_new();

	gtk_widget_set_hexpand(p_win->g_container, true);
	gtk_widget_set_vexpand(p_win->g_container, false);

	p_win->g_widget = gtk_application_window_new(g_app);
	gtk_window_set_default_size(GTK_WINDOW(p_win->g_widget), dimensions.x, dimensions.y);
	gtk_window_set_title(GTK_WINDOW(p_win->g_widget), title);

	/*
	GtkEventController *key_event = gtk_event_controller_key_new();


	gtk_event_controller_key_set_im_context(GTK_EVENT_CONTROLLER_KEY(key_event), NULL);
	gtk_widget_add_controller(p_win->g_widget, key_event);
	*/


	gtk_window_set_child(GTK_WINDOW(p_win->g_widget), p_win->g_container);
	gtk_window_present(GTK_WINDOW(p_win->g_widget));

	return p_win;
}

void gc_win_set_scene(GCWin *p_win, GCScene *scene) {
	if (p_win->curr_scene) {
		gtk_grid_remove(GTK_GRID(p_win->g_container), p_win->curr_scene->scene_container.gc_widget.g_widget);
		p_win->curr_scene = NULL;
	}

	gtk_grid_attach(
		GTK_GRID(p_win->g_container), 
		scene->scene_container.gc_widget.g_widget, 
		0, 
		0, 
		1, 
		1
	);

	p_win->curr_scene = scene;

	SceneSignalAttachedToWindow signal = {
		p_win,
	};

	listener_emit(&scene->listener, SceneSignalIDAttachedToWindow, &signal);
}

void gc_win_display(GCWin* p_win) {
	gtk_window_present(GTK_WINDOW(p_win->g_widget));
}

void gc_win_free(GCWin* win) {
	gtk_window_destroy(GTK_WINDOW(win));
}

