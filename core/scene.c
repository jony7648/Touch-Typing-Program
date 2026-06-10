#include "scene.h"

void gc_scene_display_info(GCScene* scene) {
	printf(" Scene Info\
		\nScene Id: %d\nDimensions {%d, %d}", 
		scene->scene_id, scene->dimensions.x, scene->dimensions.y
	);
}

GCScene *gc_scene_create(unsigned short scene_id, Point dimensions) {
	GCScene* p_scene = malloc(sizeof(GCScene));

	p_scene->scene_id = scene_id;
	p_scene->dimensions.x = 7;
	p_scene->dimensions.y = 7;


	GCContainerDef container_def = {
		.common = {
			.hexpand = true,
			.vexpand = true,
			.tag = 0,
			.halign = GTK_ALIGN_FILL,
			.valign = GTK_ALIGN_FILL,
		},
		.unique = {
			.rect = {0,0, 1, 1}
		},

		.spacing = {0,0}

	};





	gc_container_init(&p_scene->scene_container, &container_def);



	//gtk_grid_set_column_homogeneous(GTK_GRID(p_scene->scene_container.gc_widget.g_widget), true);
	//gtk_grid_set_row_homogeneous(GTK_GRID(p_scene->scene_container.gc_widget.g_widget), true);

	//increase the refrence of this scene container so when we detach it from
	//a window it does not get freed automaticly by the garbage collecter
	g_object_ref(p_scene->scene_container.gc_widget.g_widget);

	
	return p_scene;
} 


