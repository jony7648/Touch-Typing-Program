#include "button.h"

static void *gc_button_free(GCWidget *p) {
	GCButton *button = (GCButton*)p;

	return button;
}

GCButton* gc_button_create(GCButtonDef def) {
	GCButton *p = malloc(sizeof(GCButton));

	GtkWidget *g_button = gtk_button_new_with_label(def.text);

	gc_widget_init(
		&p->gc_widget, 
		g_button, 
		gc_button_free, 
		GCTypeButton, 
		def.common, 
		def.unique
	);	

	return p;
}



