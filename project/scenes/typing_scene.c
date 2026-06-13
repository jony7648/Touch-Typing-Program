#include "typing_scene.h"
#include "project/project.h"
#include "project/text_loader.h"
#include "core/global_manager.h"
#include "gdk/gdk.h"



GCTextView *G_ChallengeTextView = NULL;
GCTime G_test_start_time;
bool G_test_started;

size_t G_WPM_LABEL_GLOBAL_INDEX;
size_t G_KEYBOARD_CONTAINER_INDEX;

static void start_test();

static void signal_toggle_onscreen_keyboard(GSimpleAction *action, GVariant *var, gpointer user_data) {
	GVariant *state = g_action_get_state(G_ACTION(action));
    gboolean is_active = g_variant_get_boolean(state);

	g_variant_unref(state);


	is_active = !is_active;
	g_simple_action_set_state(G_ACTION(action), g_variant_new_boolean(is_active));

	GCContainer *keyboard_container = global_manager_get(G_KEYBOARD_CONTAINER_INDEX);

	if (keyboard_container) {
		gtk_widget_set_visible(keyboard_container->gc_widget.g_widget, is_active);
	}

	printf("ON SCREEN KEYBOARD TOGGLED: %d\n", is_active);
}

static void _on_attached_to_window(GCEmitData *emit_data) {
	//setup actions
	
	SceneSignalAttachedToWindow *signal = emit_data->sig_data;
	
	GSimpleAction *select_text_action = g_simple_action_new_stateful("toggle_onscreen_keyboard", NULL, g_variant_new_boolean(FALSE));

	g_signal_connect(select_text_action, "activate", G_CALLBACK(signal_toggle_onscreen_keyboard), signal->p_win);
	g_action_map_add_action(G_ACTION_MAP(signal->p_win->g_widget), G_ACTION(select_text_action));
}

static void _on_text_loader_text_loaded(GCEmitData *emit_data) {
	TextLoaderSigTextLoaded *signal = emit_data->sig_data;

	G_test_started = false;


	if (G_ChallengeTextView) {
		gc_text_view_set_text(G_ChallengeTextView, signal->text);
	}
}

static void _on_text_loader_text_reloaded(GCEmitData *emit_data) {
	TextLoaderSigTextReloaded *signal = emit_data->sig_data;

	//do not need to reset text buffer with this as it is being reset
	//with the by the input field
}


static void _on_text_loader_text_finished(GCEmitData *emit_data) {
	G_test_started = false;	

	TextLoaderSigTestFinished *signal = emit_data->sig_data;

	gctime_time_display_info(&G_test_start_time);
	gctime_time_display_info(&signal->end_time);
	float wpm = gctime_get_wpm(&G_test_start_time, &signal->end_time, signal->char_count);

	GCWidget *wpm_label = global_manager_get(G_WPM_LABEL_GLOBAL_INDEX);

	//TEST TO SEE IF THE WIDGET EXIST
	gc_widget_display_info(wpm_label);

	char wpm_str[20] = "WPM: ";
	char num_str[100];
	num_str[0] = '\0';

	cstr_from_float(num_str, wpm, 2);

	strcat(wpm_str, num_str);

	gtk_label_set_text(GTK_LABEL(wpm_label->g_widget), wpm_str);
}


static gboolean action_reset_text_field_text(gpointer ptr) {
	GCTextField *text_field = (GCTextField*)ptr;

	gtk_editable_set_text(GTK_EDITABLE(text_field->gc_widget.g_widget), "");

	return G_SOURCE_REMOVE;
}


static void _on_textfield_activate(GEmitData *emit_data) {
	GCTextField *text_field = (GCTextField*)emit_data->holder;

	const char *text = gtk_editable_get_text(GTK_EDITABLE(text_field->gc_widget.g_widget));


	if (!G_test_started) {
		start_test();
	}

	StrView view;
	str_view_init(&view, text);

	if (str_view_ends_with(&view, ' ')) {
		if (text_loader_check_for_matching_word(text)) {
			g_idle_add(action_reset_text_field_text, text_field);
			GtkTextBuffer *buffer = gtk_text_view_get_buffer(GTK_TEXT_VIEW(G_ChallengeTextView->gc_widget.g_widget));

			char *new_ptr = text_loader_advance_word();

			gtk_text_buffer_set_text(buffer, new_ptr, -1);
		}
	}


}

static void start_test() {
	gctime_get_current_time(&G_test_start_time);
	G_test_started = true;
}


static GCContainer *create_keyboard_keys(KeyboardLayout *keyboard_layout, Rect container_rect) {
	GCContainer *key_container = NULL;

	//gtk_widget_set_hexpand(container->gc_widget.g_widget, true);
	GCContainerDef key_container_def = {
		.spacing = {3, 3},
		.common = {
			.tag = 0,
			.hexpand = true,
			.vexpand = true,
			.halign = GTK_ALIGN_FILL,
			.valign = GTK_ALIGN_FILL,
		},

		.unique = {
			.rect = container_rect,
		},
	};

	GCWidgetDefCommon common_key_def = {
		.tag = 0,
		.hexpand = true,
		.vexpand = true,
		.halign = GTK_ALIGN_FILL,
		.valign = GTK_ALIGN_FILL,
	};

	key_container = gc_container_create(key_container_def);



	for (int i = 0; i<KEYBOARD_KEY_COUNT; i++) {
		char let = i + 'A';

		printf("%s\n", keyboard_layout->keys[i].data);
		GCButtonDef def = {
			.text = keyboard_layout->keys[i].data,
			.common = common_key_def,
			.unique = {
				.rect = {i%KEYBOARD_KEYS_PER_ROW, i/KEYBOARD_KEYS_PER_ROW, 1, 1},
			}
		};

		GCButton *button = gc_button_create(def);

		gc_container_attach(key_container, &button->gc_widget);
	}

	gc_container_display(key_container);

	
	return key_container;

}

static GCTextField *create_text_field(Rect rect) {
	GCTextField *text_field = NULL;

	GCTextFieldDef field_def = {
		.common = {
			.halign = GTK_ALIGN_BASELINE_CENTER,
			.valign = GTK_ALIGN_END,
			.hexpand = false,
			.vexpand = false,
			.tag = TagTypingTextField,
		},
		.unique = {
			.rect = rect,
		},

		.initial_text = "Type the above text here",
	};

	text_field = gc_text_field_create(field_def);

	listener_glisten(
		text_field->gc_widget.listener, 
		text_field->gc_widget.g_widget, 
		"changed", 
		_on_textfield_activate
	);

	text_loader_listen_for(TextLoaderSigIDTestFinished, _on_text_loader_text_finished);

	if (!text_field) {
		return NULL;
	}

	return text_field;
}

GCTextView *create_challenge_text_view(Rect rect) {
	GCTextView *text_view = gc_text_view_create((GCTextViewDef) {
		.common = {
			.halign = GTK_ALIGN_CENTER,
			.valign = GTK_ALIGN_CENTER,
			.hexpand = true,
			.vexpand = true,
			.tag = 0
		},

		.unique = {
			.rect = rect,
		},

		.init_text = "",
		.editable = false,
		.wrap_mode = GTK_WRAP_NONE,
	});		


	if (!text_view) {
		return NULL;
	}

	gtk_widget_set_size_request(text_view->gc_widget.g_widget, 300, 300);


	return text_view;
}

static GCScrollWindow *create_challenge_scroll_window(Rect rect, Point win_dimensions) {
	text_loader_listen_for(TextLoaderSigIDTextLoaded, _on_text_loader_text_loaded);
	text_loader_listen_for(TextLoaderSigIDTextReloaded, _on_text_loader_text_reloaded);


	GCScrollWindow *challenge_scroll_window = NULL;

	challenge_scroll_window = gc_scroll_window_create((GCScrollWindowDef) {
		.common = {
			.halign = GTK_ALIGN_BASELINE_CENTER,
			.valign = GTK_ALIGN_BASELINE_CENTER,
			.hexpand = true,
			.vexpand = true,
			.tag = 0,
		},
		
		.unique = {
			.rect = rect,
		},
	});

	if (!challenge_scroll_window) {
		return NULL;
	}



	gtk_widget_set_size_request(challenge_scroll_window->gc_widget.g_widget, win_dimensions.x, win_dimensions.y);

	G_ChallengeTextView = create_challenge_text_view((Rect){0,-1,1,1});
	GCWidgetSliceAppend(&challenge_scroll_window->children_slice, &G_ChallengeTextView->gc_widget);


	gc_scroll_window_present(challenge_scroll_window);


	return challenge_scroll_window;
}

static GCList* create_text_select_dropdown(Rect rect) {
	GCList *list = gc_list_create((GCListDef){
		.common = {
			.halign = GTK_ALIGN_BASELINE_CENTER,
			.valign = GTK_ALIGN_BASELINE_CENTER,
			.hexpand = true,
			.vexpand = true,
			.tag = 0,
		},

		.unique = {
			.rect = rect,

		},

		.string_arr = (const char *[]){"Hello", "Hi There", NULL},
	});


	file_util_add_files_list(list->string_list, "project_data/texts", ".txt");

	return list;
}

static GCContainer *create_keyboard_section_container(KeyboardLayout *layout) {
	GCContainer *keyboard_container = create_keyboard_keys(layout, (Rect){0,3,1,1});

	GCContainer *container = gc_container_create((GCContainerDef){
		.common = {
			.hexpand = true,
			.vexpand = true,
			.halign = GTK_ALIGN_FILL,
			.valign = GTK_ALIGN_FILL,
			.tag = 0,
		},

		.unique = {
			.rect = {0, 5, 1, 1},
		}
	});


	gc_container_attach(container, &keyboard_container->gc_widget);
	gc_container_display(container);

	 GCSizeTOptional optional = global_manager_add(keyboard_container);

	 if (optional.valid) {
		G_KEYBOARD_CONTAINER_INDEX = optional.value;
	 }
	

	return container;
}


GMenu *create_side_menu_model() {
	GtkBuilder *builder = gtk_builder_new();
	
	char file_path_buffer[1000];
	file_path_buffer[0] = '\0';

	char *cstr_arr[] = {MENU_TEMPLATE_DIR, TEMPLATE_TYPING_SCENE_UI_NAME, NULL};

	cstr_combine(file_path_buffer, sizeof(file_path_buffer), cstr_arr);

	GError *error = NULL;

	gtk_builder_add_from_file(builder, file_path_buffer, &error);

	if (error) {
        g_printerr("Error loading UI file: %s\n", error->message);
        g_clear_error(&error);
        g_object_unref(builder);
        //g_object_unref(main_menu);
    }


	GMenuModel *template_model = G_MENU_MODEL(gtk_builder_get_object(builder, "right-sidebar-menu"));
	//GMenu *template_menu = G_MENU(util_get_submenu_from_model(template_model, "right-sidebar"));


	
	GMenu *main_menu = g_menu_new();
	//GMenu *texts_menu = G_MENU(gtk_builder_get_object(builder, "my_target_section"));
	GMenu *texts_menu = g_menu_new();

	GtkStringList *string_list = gtk_string_list_new(NULL);

	file_util_add_files_list(string_list, TYPING_TEXTS_DIR, ".txt");


	for (int i=0; true; i++) {
		const char *curr_str = gtk_string_list_get_string(string_list, i);


		if (!curr_str) {
			break;
		}

		GMenuItem *item = g_menu_item_new(curr_str, NULL);
		GVariant *data = g_variant_new_string(curr_str);

		g_menu_item_set_action_and_target_value(item, "app.select_text", data);


		g_menu_append_item(texts_menu, item);

		g_object_unref(item);
	}



	/*
	if (template_menu) {
		//g_menu_append_section(main_menu, "", G_MENU_MODEL(template_menu));
	}
	*/

	g_menu_append_section(main_menu, "Options", template_model);
	g_menu_append_submenu(main_menu, "Texts", G_MENU_MODEL(texts_menu));


	g_object_unref(builder);

	return main_menu;

}

GCWidget *create_menu_button(Rect rect) {
	GCWidget *menu_button = malloc(sizeof(GCWidget));


	gc_widget_init(
		menu_button,
		gtk_menu_button_new(),
		NULL,
		0,
		(GCWidgetDefCommon) {
			.halign=GTK_ALIGN_FILL,
			.valign=GTK_ALIGN_FILL,
			.hexpand=true,
			.vexpand=true,
			.tag=0,
		},
		(GCWidgetDefUnique) {
			.rect = rect,
		}
	);


	GMenu *g_menu = create_side_menu_model();

	gtk_menu_button_set_menu_model(GTK_MENU_BUTTON(menu_button->g_widget), G_MENU_MODEL(g_menu));

	return menu_button;

}

GCContainer *create_side_menu(Rect rect) {
	Point size_request = {50,50};

	GCContainer *sidebar_container = gc_container_create((GCContainerDef) {
		.common = {
			.halign = GTK_ALIGN_END,
			.valign = GTK_ALIGN_END,
			.hexpand = false,
			.vexpand = false,
			.tag = 0,
		},

		.unique = {
			.rect = rect,
		},

		.spacing = 1
	});



	GCWidget *menu_button = create_menu_button((Rect){1,0,1,1});

	gc_container_attach(sidebar_container, menu_button);
	gc_container_display(sidebar_container);

	//gtk_widget_set_size_request(sidebar_container->gc_widget.g_widget, size_request.x, size_request.y);

	return sidebar_container;

}

GCContainer *create_notification_container(Rect rect) {
	GCContainer *container = gc_container_create((GCContainerDef){
		.common = {
			.halign = GTK_ALIGN_FILL,
			.valign = GTK_ALIGN_FILL,
			.hexpand = true,
			.vexpand = true,
			.tag = 0,
		},

		.unique = {
			rect=rect
		},

		.spacing = 1,
	});


	GCWidget *wpm_label = gc_widget_create(&(GCWidgetDefGeneric) {
		.gc_type = GCTypeLabel,
		.g_widget = gtk_label_new("WPM: "),
		.common = {
			.halign = GTK_ALIGN_START,
			.valign = GTK_ALIGN_FILL,
			.hexpand = true,
			.vexpand = true,
			.tag = 0,
		},
		.unique = {
			.rect = rect
		},
	});

	gc_container_attach(container, wpm_label);
	gc_container_display(container);

	GCSizeTOptional optional = global_manager_add(wpm_label);



	if (optional.valid) {
		G_WPM_LABEL_GLOBAL_INDEX = optional.value;
	}

	return container;
}

GCContainer *create_main_challenge_area(Rect area_rect, Slice *layout_slice) {
	enum Heights {
		TextWinHeight,
	};

	KeyboardLayout *p_keyboard_layout = NULL;
	GCScrollWindow *challenge_scroll_window = NULL;	
	GCTextField *p_text_field = NULL;
	GCContainer *keyboard_section_container = NULL;
	GCContainer *notification_container = NULL;

	GCContainer *main_area = gc_container_create((GCContainerDef) {
		.common = {
			.halign = GTK_ALIGN_CENTER,
			.valign = GTK_ALIGN_CENTER,
			.hexpand = true,
			.vexpand = true,
			.tag = 0,
		},

		.unique = {
			.rect = area_rect,
		},
		.spacing = 1,
	});


	p_keyboard_layout = &layout_slice->p_mem[0];

	challenge_scroll_window = create_challenge_scroll_window((Rect){0,0,1,1}, (Point) {300,50});
	p_text_field = create_text_field((Rect){0,1,1,1});

	notification_container = create_notification_container((Rect) {0,-1,1,1});
	keyboard_section_container = create_keyboard_section_container(p_keyboard_layout);


	gc_container_attach(main_area, &challenge_scroll_window->gc_widget);

	gc_container_attach(main_area, &p_text_field->gc_widget);

	gc_container_attach(main_area, &keyboard_section_container->gc_widget);

	gc_container_attach(main_area, &notification_container->gc_widget);

	gc_container_display(main_area);


	return main_area;
}


GCScene* create_test_scene(Slice *layout_slice) {
	enum Heights {
		SidebarHeight,
		ChallengeHeight,
	};

	Point dimensions = {300,300};
	GCScene *p_scene = gc_scene_create(1, dimensions);

	GCContainer *challenge_area = create_main_challenge_area((Rect){0,ChallengeHeight,1,1}, layout_slice);

	GCContainer *sidebar_container = create_side_menu((Rect){0,SidebarHeight,1,1});


	gc_container_attach(&p_scene->scene_container, &challenge_area->gc_widget);
	gc_container_attach(&p_scene->scene_container, &sidebar_container->gc_widget);

	listener_listen(&p_scene->listener, SceneSignalIDAttachedToWindow, _on_attached_to_window);

	return p_scene;
}
