#include "gc_widget.h"



void gc_widget_display_info(GCWidget *p) {
	g_print("\nWidget Info\nGWidget: %p\nWidgetType: %d\nDestructorPtr: %p\nListener: %p\n\n",
		p->g_widget, p->widget_type, p->destructor_ptr, p->listener
	);
}

void gc_widget_init(GCWidget* widget, GtkWidget* gtk_widget, void *(*destructor_ptr)(GCWidget*), GCType type, GCWidgetDefCommon com, GCWidgetDefUnique unique) {
	widget->g_widget = gtk_widget;
	widget->rect = unique.rect;
	widget->widget_type = type;
	widget->tag = com.tag;
	widget->is_presenting = FALSE;
	widget->destructor_ptr = destructor_ptr;
	widget->listener = NULL;

	gtk_widget_set_halign(gtk_widget, com.halign);
	gtk_widget_set_valign(gtk_widget, com.valign);
	gtk_widget_set_hexpand(gtk_widget, com.hexpand);
	gtk_widget_set_vexpand(gtk_widget, com.vexpand);
}

GCError gc_widget_free(GCWidget *p) {
	//This function is responsible for freeing the GCWidget
	//This will also free if the widget if it does not have a destructor
	//in case if a type is not used frequently
	void *heap_mem = p;

	if (!p) {
		return ErrorNull;
	}

	if (p->destructor_ptr) {
		heap_mem = p->destructor_ptr(p);
	}
	

	//do gc free stuff


	if (heap_mem) {
		free(heap_mem);
	}



	if (p->listener) {
		listener_close(p->listener);
		free(p->listener);
		p->listener = NULL;
	}

	return ErrorClear;
}
