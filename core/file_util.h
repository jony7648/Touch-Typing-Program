#pragma once
#include "error.h"

#include <gtk/gtk.h>
#include <stdio.h>

typedef struct {
	char **str_arr;
	size_t size;
	size_t count;
} FileList;

void file_util_get_dir_files(FileList *file_list, char *path);
GCError file_util_add_files_list(GtkStringList *string_list, char *dir_path, char *file_extension);
