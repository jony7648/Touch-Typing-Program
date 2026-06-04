#include "app.h"
#include "window.h"
#include "keyboard.h"

#include <stdlib.h>




GCWin *gc_win_create(GtkApplication* g_app, char *title, Point dimensions) {
	GCWin *p_win = malloc(sizeof(GCWin));
	p_win->title = title;
	p_win->scene_id = -1;
	p_win->dimensions = dimensions;

	p_win->g_widget = gtk_application_window_new(g_app);
	gtk_window_set_default_size(GTK_WINDOW(p_win->g_widget), dimensions.x, dimensions.y);
	gtk_window_set_title(GTK_WINDOW(p_win->g_widget), title);

	GtkEventController *key_event = gtk_event_controller_key_new();


	gtk_event_controller_key_set_im_context(GTK_EVENT_CONTROLLER_KEY(key_event), NULL);
	gtk_widget_add_controller(p_win->g_widget, key_event);



	return p_win;
}

void gc_win_display(GCWin* p_win) {
	gtk_window_present(GTK_WINDOW(p_win->g_widget));
}

void gc_win_free(GCWin* win) {
	gtk_window_destroy(GTK_WINDOW(win));
}
