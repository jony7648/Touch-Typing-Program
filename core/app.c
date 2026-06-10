#include "space.h"
#include "app.h"
#include "slice.h"
#include "space.h"

#include <gtk/gtk.h>

GCApp *G_app = NULL;

void gc_app_init(GCApp *p_app, int argc, char **argv) {
	p_app->g_component = gtk_application_new(p_app->app_id, G_APPLICATION_DEFAULT_FLAGS);
	p_app->dimensions.x = 30;
	p_app->dimensions.y = 30;
	p_app->argc = argc;
	p_app->argv = argv;
	p_app->status = 0;
	p_app->win_arr.size = 0;
}

void gc_app_create_wins(GCApp *p, unsigned int win_count) {
	Point dimensions = {300,300};
	p->win_arr.size = win_count;

	p->win_arr.p_mem = malloc(sizeof(GCWin*) * win_count);

	for (int i=0; i<win_count; i++) {
		p->win_arr.p_mem[i] = gc_win_create(p->g_component, "Test Win", dimensions);
	}
}

void gc_app_run(GCApp *p_app, void(*activate_func)(GtkApplication*, gpointer), void* activate_signal) {
	G_app = p_app;
	g_signal_connect(p_app->g_component, "activate", G_CALLBACK(activate_func), activate_signal);

	p_app->status = g_application_run(G_APPLICATION(p_app->g_component), p_app->argc, p_app->argv);
}

GCError gc_app_attach_scenes(GCApp *p, GCScene **scene_arr, int scene_count) {
	p->scene_arr.p_mem = malloc(sizeof(GCScene*) * scene_count);

	int null_count = 0;


	GCError err = ErrorClear;

	for (int i=0; i<scene_count; i++) {
		if (!scene_count) {
			null_count++;
			err = ErrorNull;
			continue;
		}

		p->scene_arr.p_mem[i - null_count] = scene_arr[i];
	}

	scene_count -= null_count;

	p->scene_arr.size = scene_count;

	return ErrorClear;
}

GCScene *get_scene_by_id(GCApp *p, int scene_id) {
	GCScene *scene = NULL;

	for (int i=0; i<p->scene_arr.size; i++) {
		if (p->scene_arr.p_mem[i]->scene_id == scene_id) {
			scene = p->scene_arr.p_mem[i];	
		}
	}

	return scene;
}

GCWin *get_win_by_id(GCApp *p, int win_id) {
	GCWin *gc_win = NULL;

	if (win_id < 0 || win_id >= p->win_arr.size) {
		return NULL;
	}

	gc_win = p->win_arr.p_mem[win_id];
	
	return gc_win;
}


GCError gc_app_display_scene(int win_id, int scene_id) {
	if (!G_app) {
		return ErrorNull;
	}

	GCWin *win = get_win_by_id(G_app, win_id);	
	GCScene *scene = NULL;

	if (!win) {
		return ErrorNull;
	}

	scene = get_scene_by_id(G_app, scene_id);



	if (!scene) {
		return ErrorNull;
	}



	gc_win_set_scene(win, scene);
	gc_container_display(&scene->scene_container);

	return ErrorClear;
}

/*
GCError gc_app_display_scene(GCApp *p, int scene_id) {
	GCScene *scene = NULL;

	scene = get_scene_by_id(p, scene_id);
	
	if (scene == NULL) {
		return ErrorSceneIndexNotFound;
	}

	GCWin *scene_win = NULL;
	
	//loop from the back to start filling windows from the front
	//as we are check from the back to allow for the checking for 
	//empty wins and wins with the same index at the same time
	for (int i=p->win_arr.size-1; i>=0; i--) {

		GCWin *win = p->win_arr.p_mem[i]; 
		printf("%d\n", win->scene_id);
		puts("check");
		
		if (win->scene_id == scene_id) {
			return ErrorClear;
		}
	
	
		if (win->scene_id == -1) {
			scene_win = win;
		}
	}


	if (!scene_win) {
		return ErrorNoWinForScene;
	}


	gc_container_display(&scene->scene_container);
	gtk_window_set_child(
		GTK_WINDOW(scene_win->g_widget),		
		scene->scene_container.gc_widget.g_widget
	);


	gtk_application_window_set_show_menubar(GTK_WINDOW(scene_win->g_widget), true);

	gtk_window_set_default_size(GTK_WINDOW(scene_win->g_widget), scene->dimensions.x, scene->dimensions.y);
	gtk_window_present(GTK_WINDOW(scene_win->g_widget));


	return ErrorClear;
}
*/
GCError gc_app_switch_scenes(GCApp *app, GCWin *win, int scene_id) {
	if (!G_app) {
		return ErrorNull;
	}

	GCScene *scene = get_scene_by_id(app, scene_id);

	if (!scene) {
		return ErrorSceneIndexNotFound;
	}

	//make it so if aleready presenting cancel
	

	return ErrorClear;
	
}
