#pragma once
#include "types.h"
#include "core/app.h"

#include <stdio.h>
void global_manager_init(GCApp*);
void *global_manager_get(size_t index);
GCSizeTOptional global_manager_add(void *object);
