#include "typing_scene.h"
#include "gc_widgets/include_me.h"
#include "project/project.h"
#include "project/text_loader.h"
#include "gdk/gdk.h"

GCTextView *G_ChallengeTextView = NULL;


static void _on_text_loader_text_loaded(GCEmitData *emit_data) {
	TextLoaderSigTextLoaded *signal = emit_data->sig_data;


	if (G_ChallengeTextView) {
		gc_text_view_set_text(G_ChallengeTextView, signal->text);
	}
}

static void _on_text_loader_text_reloaded(GCEmitData *emit_data) {
	TextLoaderSigTextReloaded *signal = emit_data->sig_data;

	//do not need to reset text buffer with this as it is being reset
	//with the by the input field


}

static gboolean action_reset_text_field_text(gpointer ptr) {
	GCTextField *text_field = (GCTextField*)ptr;

	gtk_editable_set_text(GTK_EDITABLE(text_field->gc_widget.g_widget), "");

	return G_SOURCE_REMOVE;
}


static void _on_textfield_activate(GEmitData *emit_data) {
	GCTextField *text_field = (GCTextField*)emit_data->holder;

	const char *text = gtk_editable_get_text(GTK_EDITABLE(text_field->gc_widget.g_widget));

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

	return container;
}


GMenu *create_side_menu_model() {
	GMenu *main_menu = g_menu_new();
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

	g_menu_append_submenu(main_menu, "Texts", G_MENU_MODEL(texts_menu));


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


GCScene* create_test_scene(Slice *layout_slice) {

	Point dimensions = {300,300};
	GCScene *p_scene = gc_scene_create(1, dimensions);
	GCTextField *p_text_field = NULL;
	GCScrollWindow *challenge_scroll_window = NULL;	



	KeyboardLayout *p_keyboard_layout = &layout_slice->p_mem[0];



	GCContainer *keyboard_section_container = create_keyboard_section_container(p_keyboard_layout);
	challenge_scroll_window = create_challenge_scroll_window((Rect){0,0,1,1}, (Point) {300,50});
	p_text_field = create_text_field((Rect){0,1,1,1});






	//list testing

	GCContainer *sidebar_container = create_side_menu((Rect){1,0,1,1});


	gc_container_attach(&p_scene->scene_container, &sidebar_container->gc_widget);





	gc_container_attach(&p_scene->scene_container, &p_text_field->gc_widget);
	if (challenge_scroll_window) {
		gc_container_attach(&p_scene->scene_container, &challenge_scroll_window->gc_widget);
	}

	gc_container_attach(&p_scene->scene_container, &keyboard_section_container->gc_widget);



	//const char *challenge_text = text_loader_load_text("project_data/texts/bob text 1.txt");

	return p_scene;
}
