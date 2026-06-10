#pragma once
#include "gc_widgets/gc_types.h"
#include "core/types.h"
#include "core/error.h"

#include <gtk/gtk.h>


typedef struct GEmitData GEmitData;
typedef struct GCWidget GCWidget;
typedef struct GCEmitData GCEmitData;


typedef void (*GCEmitFuncPtr)(GCEmitData*);

struct GCEmitData {
	GCType type;	
	void *holder;
	void *sig_data;
};


struct GEmitData {
	GCWidget *holder;
	void (*_func_ptr)(GEmitData*);
};

typedef struct {
	void* obj;
} Signal;


typedef struct {
	GEmitData *emit_data_arr;
	size_t count;
	size_t size;
} GEmitDataHolder;



typedef struct {
	void *(*func_arr)(GCEmitData*);	
	size_t count;
	size_t max_count;
} ListenerFuncArr;

typedef struct {
	GCEmitFuncPtr *p_mem;	
	size_t count;
	size_t size;
	int id;
} GCEmitFuncList;


typedef struct {
	GCType parent_type;
	void *parent_obj;
	GCEmitFuncList *_emit_func_arr;
	int _signal_count;

	GEmitDataHolder _emit_data_holder;
} Listener;



GCError listener_init(Listener *listener, void *parent_obj, GCType type, int signal_count);
void listener_listen(Listener *sending_listener, int signal_id, GCEmitFuncPtr func_ptr);
void listener_glisten(Listener *listener, GtkWidget *g_widget, char *signal_action, void (*func_ptr)(GEmitData*));
void listener_emit(Listener *listener, int id, void *signal_data);
void listener_close(Listener *listener);
