#pragma once
#include "gc_widgets/gc_types.h"
#include "core/types.h"

#include <gtk/gtk.h>

typedef struct GEmitData GEmitData;
typedef struct GCWidget GCWidget;

typedef struct {
	GCType type;	
	void* holder;
	void* emitter;
} GCEmitData;


struct GEmitData {
	GCWidget *holder;
	void (*_func_ptr)(GEmitData*);
};

typedef struct {
	void* obj;
} Signal;


typedef struct {
	VoidFuncPtr *func_arr;
	size_t count;
} GCEmitFunctions;


typedef struct {
	GEmitData *emit_data_arr;
	size_t count;
	size_t size;
} GEmitDataHolder;

typedef struct {
	GCType parent_type;
	void *parent_obj;
	GCEmitFunctions *_emit_func_arr;
	int _signal_count;

	GEmitDataHolder _emit_data_holder;
} Listener;



void listener_init(Listener *listener, void *parent_obj, GCType type, int signal_count);
void listener_listen(Listener *sending_listener, int signal_id, VoidFuncPtr func_ptr);
void listener_glisten(Listener *listener, GtkWidget *g_widget, char *signal_action, void (*func_ptr)(GEmitData*));
void listener_free(Listener *listener);
