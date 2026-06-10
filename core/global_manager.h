#pragma once
#include "types.h"
#include "core/app.h"

#include <stdio.h>
void global_manager_init(GCApp*);
GCSizeTOptional global_mananger_add(void *object);
