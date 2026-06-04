#include "scroll_window.h"

void *gc_scroll_window_free(GCWidget *gc) {
	GCScrollWindow *window = (GCScrollWindow*)gc;	
	return gc;
}

GCScrollWindow *gc_scroll_window_create(GCScrollWindowDef def) {
	GCScrollWindow *window = malloc(sizeof(GCScrollWindow));

	GCWidgetSliceInit(&window->children_slice, 5);

	GtkWidget *g_widget = gtk_scrolled_window_new();

	gc_widget_init(
		&window->gc_widget,
		g_widget,
		gc_scroll_window_free,
		GCTypeScrollWindow,
		def.common,
		def.unique
	);

	return window;
}

void gc_scroll_window_present(GCScrollWindow *p) {
	for (size_t i=0; i<p->children_slice.count; i++) {
		GCWidget *gc = p->children_slice.p_mem[i];
		gtk_scrolled_window_set_child(GTK_SCROLLED_WINDOW(p->gc_widget.g_widget), gc->g_widget);
	}
}
