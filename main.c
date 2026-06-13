#include "core/arena.h"
#include "core/string.h"
#include "core/time_componet.h"
#include "core/keyboard.h"
#include "core/str_view.h"
#include "core/string.h"

#include "core/types.h"
#include "core/space.h"
#include "core/scene.h"
#include "core/timer.h"

#include "core/util.h"
#include "core/file_util.h"

#include "core/app.h"
#include "core/window.h"

#include "core/slice.h"

#include "gc_widgets/button.h"
#include "gc_widgets/container.h"

#include "project/project.h"
#include "project/paths.h"

#include "core/global_manager.h"
#include "project/scenes/typing_scene.h"
#include "project/text_loader.h"

#include <gtk/gtk.h>

#include <locale.h>
#include <dirent.h>


static void _on_text_selected(GSimpleAction *action, GVariant *var, gpointer user_data) {
	const char *text_name = g_variant_get_string(var, NULL);
	const char *file_extension = ".txt";
	
	const char *path_list[] = {
		TYPING_TEXTS_DIR,
		text_name,
		file_extension,
	};

	const size_t path_list_count = sizeof(path_list) / sizeof(path_list[0]);

	const char *text_path = util_append_c_strs(path_list, path_list_count);


	text_loader_load_text(text_path);

	free(text_path);

}




static void load_application_menu(GCApp *gc_app, GCWin *gc_win) {
	GtkBuilder *builder = gtk_builder_new_from_file("menu.ui");
	GMenuModel *main_menu_model = G_MENU_MODEL(gtk_builder_get_object(builder, "main-menu"));
	//GMenuModel *texts_menu_model = NULL;
	GMenu *main_menu = NULL;
	GMenu *texts_menu = NULL;

	GtkStringList *string_list = gtk_string_list_new(NULL);



	GSimpleAction *select_text_action = g_simple_action_new("select_text", G_VARIANT_TYPE_STRING);

	g_signal_connect(select_text_action, "activate", G_CALLBACK(_on_text_selected), gc_app);
	g_action_map_add_action(G_ACTION_MAP(gc_win->g_widget), G_ACTION(select_text_action));


	file_util_add_files_list(string_list, TYPING_TEXTS_DIR, ".txt");


	main_menu = g_menu_new();


	//texts_menu_model = get_submenu_from_model(main_menu_model, "Texts");
	texts_menu = G_MENU(gtk_builder_get_object(builder, "typing_texts_section"));


	int str_index = 0;
	char *curr_str = NULL;

	GMenuItem *item = g_menu_item_new(curr_str, NULL);
	GVariant *data = g_variant_new_string("nug");

	// Now GLib can safely look up the already-registered action and map the target type
	g_menu_item_set_action_and_target_value(item, "win.select_text", data);

	g_menu_append_item(texts_menu, item);

	do {
		curr_str = gtk_string_list_get_string(string_list, str_index);
		str_index++;

		printf("Str: %s\n", curr_str);

		if (!curr_str) {
			break;
		}

		GMenuItem *item = g_menu_item_new(curr_str, NULL);
		GVariant *data = g_variant_new_string("nug");

		g_menu_item_set_action_and_target_value(item, "win.select_text", data);


		g_menu_append_item(texts_menu, item);

		g_object_unref(item);

	} while(0);



	g_menu_append_section(main_menu, "my-app", main_menu_model);

	gtk_application_set_menubar(GTK_APPLICATION(gc_app->g_component), G_MENU_MODEL(main_menu));

	g_object_unref(builder);
}


static void load_application_actions(GCApp *app) {
	GSimpleAction *select_text_action = g_simple_action_new("select_text", G_VARIANT_TYPE_STRING);

	g_signal_connect(select_text_action, "activate", G_CALLBACK(_on_text_selected), app);
	g_action_map_add_action(G_ACTION_MAP(app->g_component), G_ACTION(select_text_action));

}

static void signal_app_run(GtkApplication *g_app, gpointer p_signal) {
	LaunchData *launch_data = (LaunchData*)p_signal;
	Slice *p_layout_slice = launch_data->p_layout_slice;


	GCApp *p_app = launch_data->p_app;



	Point dimensions={600,600};

	gc_app_create_wins(p_app, 1);

	Point default_demensions = {0,0};

	//load_application_menu(p_app, p_app->win_arr.p_mem[0]);

	GCError err = 0;

	GCScene *scene_arr[] = {
		create_test_scene(p_layout_slice),
	};



	
	int scene_count = sizeof(scene_arr)/sizeof(GCScene*);

	err = gc_app_attach_scenes(p_app, scene_arr, scene_count);


	err = gc_app_display_scene(0, 1);


	GCContainerSignalTagCallParameters call_parameters = {
		.container = &scene_arr[0]->scene_container,
		.signal_id = 0,
		.signal_data = NULL,
		.widget_type = GCTypeButton,
		.tag = 3,
	};

	gc_container_signal_tag_call(&call_parameters);

	switch (err) {
		case ErrorNoWinForScene:
			puts("There are no available windows for this scene");
			break;
		case ErrorOutOfBounds:
			puts("Invalid scene index");
			break;
		case ErrorSceneIndexNotFound:
			puts("Scene of that index could not be found");
			break;
		case ErrorClear:
			puts("Successfully displayed the scene");
			break;
		default:
			puts("Unknown Error Encountered");
			break;
	}

	load_application_actions(p_app);
}



GCError load_keyboard_layouts(char *dir_path, Slice *layout_slice) {
	struct dirent *entry;

	DIR *p_dir = opendir(dir_path);

	if (!p_dir) {
		return ErrorFileAccessFailed;
	}


	StrView extension;


	char layout_path[100];
	int max_copy_chars = sizeof(layout_path) - strlen(KEYBOARD_LAYOUT_DIR);


	while ((entry = readdir(p_dir)) != NULL) {
		str_view_init(&extension, entry->d_name);

		str_view_get_extension(&extension);


		if (str_view_cmp(&extension, KEYBOARD_LAYOUT_EXTENSION) == 0) {
			KeyboardLayout *layout = SliceRequest(layout_slice);

			if (!layout) {
				return ErrorNull;
			}

			strncpy(layout_path, KEYBOARD_LAYOUT_DIR, sizeof(layout_path));
			strncat(layout_path, entry->d_name, max_copy_chars);


			GCError err = keyboard_layout_load(layout, layout_path);

			if (err) {
				printf("Error loading keyboard layout\n");	
			}
		}

		
	}

	return ErrorClear;
}


void timeout_func(TimerSignalTimeout*) {
	puts("Timeout func executed");
}

int main(int argc, char **argv) {
	GCDate date;	

	gctime_set_date_from_sys_clock(&date);


	setlocale(LC_ALL, ".ut8");

	GCError err = 0;

	Slice layout_slice;
	GCApp app={
		.name = "Test App", 
		.app_id = "org.jony.test",
	};

	SliceInit(&layout_slice, sizeof(KeyboardLayout), 5);
	err = load_keyboard_layouts(KEYBOARD_LAYOUT_DIR, &layout_slice);

	if (err) {
		return 1;
	}


	LaunchData launch_data = {
		.p_app = &app,
		.p_layout_slice = &layout_slice,
		.p_date = &date,
	};


	global_manager_init(&app);
	text_loader_init(&app);

	gc_app_init(&app, argc, argv);
	gc_app_run(&app, signal_app_run, &launch_data);

	SliceFree(&layout_slice);

	return 0;
}
