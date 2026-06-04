#include "text_field.h"

void *gc_text_field_free(GCWidget *p) {
	GCTextField *field = (GCTextField*)p;

	return field;
}

GCTextField *gc_text_field_create(GCTextFieldDef def) {
	GCTextField *p = malloc(sizeof(GCTextField));
	Listener *listener = malloc(sizeof(Listener));

	if (!p) {
		return NULL;
	}

	GtkWidget *g_field = gtk_entry_new();

	if (!g_field) {
		return NULL;
	}

	gc_widget_init(
		&p->gc_widget, 
		g_field, 
		gc_text_field_free, 
		GCTypeTextField, def.common, 
		def.unique
	);

	listener_init(
		listener, 
		p, 
		GCTypeTextField, 
		0
	);
	
	p->gc_widget.listener = listener;

	return p;
}
