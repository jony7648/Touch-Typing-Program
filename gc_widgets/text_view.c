#include "text_view.h"

void *gc_text_view_free(GCWidget *gc_widget) {
	GCTextView *text_view = (GCTextView*)gc_widget;

	return text_view;
}


GCTextView *gc_text_view_create(GCTextViewDef def) {
	GCTextView *p = malloc(sizeof(GCTextView));


	if (!p) {
		return NULL;
	}

	//GtkWidget *g_widget = gtk_text_view_new_with_buffer(text_buffer);
	GtkWidget *g_widget = gtk_text_view_new();

	if (!g_widget) {
		return NULL;	
	}

	gtk_text_view_set_wrap_mode(GTK_TEXT_VIEW(g_widget), def.wrap_mode);


	gc_widget_init(
		&p->gc_widget,
		g_widget,
		gc_text_view_free,
		GCTypeTextView,
		def.common,
		def.unique
	);

	gtk_text_view_set_editable(GTK_TEXT_VIEW(g_widget), def.editable);

	gc_text_view_set_text(p, def.init_text);

	return p;
}

void gc_text_view_set_text(GCTextView *text_view, const char *text) {
	GtkTextBuffer *text_buffer = gtk_text_view_get_buffer(GTK_TEXT_VIEW(text_view->gc_widget.g_widget));	
	gtk_text_buffer_set_text(text_buffer, text, -1);
}
