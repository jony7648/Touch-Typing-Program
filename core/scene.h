#pragma once
#include "space.h"
#include "slice.h"


#include "gc_widgets/container.h"


typedef struct {
	GCContainer scene_container;	
	unsigned short scene_id;
	Point dimensions;
} GCScene;

GCScene* gc_scene_create(unsigned short scene_id, Point dimensions);
GCError gc_scene_present(GCContainer* container);
void gc_scene_display_info(GCScene* scene);
