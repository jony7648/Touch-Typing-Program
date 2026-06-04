#include "typing_scene.h"
#include "gc_widgets/include_me.h"
#include "project/project.h"
#include "project/text_loader.h"
#include "gdk/gdk.h"

GCTextView *G_ChallengeTextView = NULL;


static gboolean reset_text_field_text(gpointer ptr) {
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
			g_idle_add(reset_text_field_text, text_field);
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

GCTextView *create_challenge_text_view(Rect rect, const char* text) {
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

		.init_text = text,
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
	const char *challenge_text = text_loader_load_text("project_data/texts/bob text 1.txt");

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

	G_ChallengeTextView = create_challenge_text_view((Rect){0,-1,1,1}, challenge_text);
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















	gc_container_attach(&p_scene->scene_container, &p_text_field->gc_widget);
	if (challenge_scroll_window) {
		gc_container_attach(&p_scene->scene_container, &challenge_scroll_window->gc_widget);
	}

	gc_container_attach(&p_scene->scene_container, &keyboard_section_container->gc_widget);


	return p_scene;
}
