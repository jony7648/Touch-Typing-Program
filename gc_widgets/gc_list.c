#include "gc_list.h"


static void setup_list_item(GtkSignalListItemFactory *factory, GtkListItem *list_item, gpointer user_data) {
	GtkWidget *label = gtk_label_new("");
	gtk_widget_set_halign(label, GTK_ALIGN_START);
	gtk_list_item_set_child(list_item, label);
}

static void bind_list_item(GtkSignalListItemFactory *factory, GtkListItem *list_item, gpointer user_data) {
	GtkWidget *label = gtk_list_item_get_child(list_item);

	GObject *item_object = gtk_list_item_get_item(list_item);

	const char *text = gtk_string_object_get_string(item_object);

	gtk_label_set_text(GTK_LABEL(label), text);
}

void *gc_list_free(GCWidget *gc) {
	GCList *list = (GCList*)gc;

	return list;
}


GCList *gc_list_create(GCListDef def) {
	GCList *p = malloc(sizeof(GCList));

	if (!p) {
		return NULL;
	}


	

	p->string_list = gtk_string_list_new(def.string_arr);

	GtkSingleSelection *selection_model = gtk_single_selection_new(G_LIST_MODEL(p->string_list));

	GtkListItemFactory *factory = gtk_signal_list_item_factory_new();
	g_signal_connect(factory, "setup", G_CALLBACK(setup_list_item), NULL);
	g_signal_connect(factory, "bind", G_CALLBACK(bind_list_item), NULL);

	GtkWidget *g_widget = gtk_list_view_new(GTK_SELECTION_MODEL(selection_model), factory);


	if (!g_widget) {
		return NULL;
	}

	gc_widget_init(
		&p->gc_widget,
		g_widget,	
		gc_list_free,
		GCTypeEmpty,
		def.common,
		def.unique
	);

	gtk_widget_set_size_request(g_widget, 300, 300);
	

	return p;
}
