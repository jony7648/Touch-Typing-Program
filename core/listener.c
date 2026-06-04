#include "listener.h"

const int STARTING_EMIT_FUCNTIONS_SIZE = 5;
const int STARTING_GEMIT_HOLDER_SIZE = 5;


static void handle_g_message(GtkApplication *g_app, gpointer p_signal) {
	GEmitData *p_emit_data = (GEmitData*)p_signal;

	if (!p_emit_data) {
		return;
	}

	p_emit_data->_func_ptr(p_emit_data);	
}


GEmitData *g_emit_data_request(GEmitDataHolder *p) {
	size_t new_count = p->count + 1;


	GEmitData *ret_ptr = NULL;

	if (p->size == 0) {
		p->size = STARTING_EMIT_FUCNTIONS_SIZE;
		p->emit_data_arr = malloc(sizeof(GEmitData) * p->size);

		if (!p->emit_data_arr) {
			return NULL;
		}

	}
	else if (new_count > p->size) {
		p->size *= 2;
		p->emit_data_arr = realloc(p->emit_data_arr, p->size * sizeof(GEmitData));	

		if (!p->emit_data_arr) {
			return NULL;
		}
	}

	ret_ptr = &p->emit_data_arr[p->count];
	p->count = new_count;

	return ret_ptr;
}

void listener_init(Listener *p, void *parent_obj, GCType type, int signal_count) {
	p->parent_obj = parent_obj;
	p->parent_type = type;
	p->_signal_count = signal_count;

	p->_emit_data_holder.size = 0;
	p->_emit_data_holder.count = 0;
	p->_emit_data_holder.emit_data_arr = NULL;

	p->_emit_func_arr = malloc(signal_count * sizeof(GCEmitFunctions));


	for (int i=0; i<p->_signal_count; i++) {
		GCEmitFunctions *func_arr = &p->_emit_func_arr[i];
		func_arr->count = STARTING_EMIT_FUCNTIONS_SIZE;
		func_arr->func_arr = malloc(func_arr->count * sizeof(VoidFuncPtr));
	}
	
}


void listener_glisten(Listener *listener, GtkWidget *g_widget, char *signal_action, void (*func_ptr)(GEmitData*)) {
	GCWidget *gc = (GCWidget*)listener->parent_obj;

	if (!gc) {
		return;
	}

	GEmitData *p_emit_data = g_emit_data_request(&listener->_emit_data_holder);

	if (!p_emit_data) {
		return;
	}

	p_emit_data->_func_ptr = func_ptr;
	p_emit_data->holder = gc;

	g_signal_connect(
		g_widget, 
		signal_action, 
		G_CALLBACK(handle_g_message), 
		p_emit_data
	);
}

void gc_emit_functions_free(GCEmitFunctions *func_multi_arr) {

}

void g_emit_data_holder_free(GEmitDataHolder *holder) {

}

void listener_free(Listener *p) {
	gc_emit_functions_free(p->_emit_func_arr);	
	g_emit_data_holder_free(&p->_emit_data_holder);

	p->_emit_func_arr = NULL;
}

