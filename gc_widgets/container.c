#include "container.h"
#include "stdlib.h"

static void *gc_container_free(GCWidget *p) {
	GCContainer *container = (GCContainer*)p;

	return container;
}

void gc_container_init(GCContainer *p, GCContainerDef *def) {
	GtkWidget *g_container = gtk_grid_new();

	gc_widget_init(
		&p->gc_widget, 
		g_container, 
		gc_container_free, 
		GCTypeContainer, 
		def->common,
		def->unique
	);
	

	if (def->spacing.x > 0) {
		gtk_grid_set_column_spacing(GTK_GRID(g_container), def->spacing.x);
	}
	if (def->spacing.y > 0) {
		gtk_grid_set_row_spacing(GTK_GRID(g_container), def->spacing.y);
	}


	GCWidgetSliceInit(&p->widget_slice, 5);
}

GCContainer *gc_container_create(GCContainerDef def) {
	GCContainer *p = malloc(sizeof(GCContainer));

	gc_container_init(p, &def);

	return p;
}

GCError gc_container_attach(GCContainer *p, GCWidget *widget) {
	GCWidgetSliceAppend(&p->widget_slice, widget);
	return ErrorClear;
}
void gc_container_signal_tag_call(GCContainerSignalTagCallParameters* parameters) {
	if (!parameters) {
		return;
	}

	for (int i=0; i<parameters->container->widget_slice.count; i++) {
		GCWidget *gc = parameters->container->widget_slice.p_mem[i];

		if (!gc) {
			return;
		}

		if (gc->widget_type == GCTypeContainer) {
			GCContainer *old = parameters->container;
			parameters->container = (GCContainer*)gc;

			gc_container_signal_tag_call(parameters);

			parameters->container = old;
		}

		if (gc->widget_type == parameters->widget_type && gc->tag == parameters->tag) {
			//DO CALL TAG STUFF PLEASSSSEEEEEEEEEEE........
		}

	}

}

GCError gc_container_display(GCContainer *p) {
	for (int i=0; i<p->widget_slice.count; i++) {
		GCWidget* child = p->widget_slice.p_mem[i];

		if (!child) {
			continue;
		}

		gtk_grid_attach(
			GTK_GRID(p->gc_widget.g_widget), 
			child->g_widget, 
			child->rect.x,
			child->rect.y,
			child->rect.h,
			child->rect.w
		);
	}

	return ErrorClear;

}



