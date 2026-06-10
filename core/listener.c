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

GCError listener_init(Listener *p, void *parent_obj, GCType type, int signal_count) {
	p->parent_obj = parent_obj;
	p->parent_type = type;
	p->_signal_count = signal_count;

	p->_emit_data_holder.size = 0;
	p->_emit_data_holder.count = 0;
	p->_emit_data_holder.emit_data_arr = NULL;



	if (signal_count < 1) {
		return ErrorClear;
	}
	 
	p->_emit_func_arr = malloc(signal_count * sizeof(GCEmitFuncList));

	if (!p->_emit_func_arr) {
		return ErrorFailedAlloc;
	}

	for (int i=0; i<signal_count; i++) {
		GCEmitFuncList *func_arr = &p->_emit_func_arr[i];

		func_arr->size = STARTING_EMIT_FUCNTIONS_SIZE;
		func_arr->count = 0;
		func_arr->id = i;
		func_arr->p_mem = malloc(sizeof(GCEmitFuncPtr) * p->_emit_func_arr[i].size);
	}

	return ErrorClear;
}

void gc_emit_functions_append(GCEmitFuncList *func_arr, GCEmitFuncPtr func) {
	size_t new_count = func_arr->count + 1;

	if (new_count > func_arr->size) {
		func_arr->size *= 2;	

		void *new_mem = realloc(func_arr->p_mem, func_arr->size);

		if (!new_mem) {
			return;
		}

		func_arr->p_mem = new_mem;
	}

	func_arr->p_mem[func_arr->count] = func;
	func_arr->count = new_count;
}


void listener_listen(Listener *sending_listener, int signal_id, GCEmitFuncPtr func_ptr) {
	if (sending_listener->_signal_count <= signal_id) {
		printf(
			"You only have %d signals in the listener at address %p. You cannot listen for the signal of id %d!\n",
			sending_listener->_signal_count, sending_listener, signal_id
		);
		return;
	}

	gc_emit_functions_append(&sending_listener->_emit_func_arr[signal_id], func_ptr);
}



void listener_emit(Listener *listener, int id, void *signal_data) {
	if (listener->_signal_count <= id) {
		printf(
			"You only have %d signals in in the listener at address %p. You cannot emit the signal of id %d!\n",
			listener->_signal_count, listener, id
		);
		return;
	}

	GCEmitData emit_data = {
		.type = listener->parent_type,
		.holder = listener->parent_obj,
		.sig_data = signal_data,
	};


	GCEmitFuncList *func_arr = &listener->_emit_func_arr[id];


	for (int i=0; i<func_arr->count; i++) {
		GCEmitFuncPtr func = func_arr->p_mem[i];

		if (func) {
			func(&emit_data);
		}
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

void gc_emit_func_list_free(GCEmitFuncList *func_arr) {
	free(func_arr->p_mem);
}

void g_emit_data_holder_free(GEmitDataHolder *holder) {
	free(holder->emit_data_arr);	
}

void listener_close(Listener *p) {
	for (int i=0; i<p->_signal_count; i++) {
		gc_emit_func_list_free(&p->_emit_func_arr[i]);	
	}

	g_emit_data_holder_free(&p->_emit_data_holder);
}

