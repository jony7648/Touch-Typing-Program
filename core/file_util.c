#include "file_util.h"
#include "str_view.h"

void file_list_init(FileList *file_list, size_t start_size) {
	file_list->size = start_size;
	file_list->count = 0;
	file_list->str_arr = 0;
}

void file_util_get_dir_files(FileList *file_list, char *path) {

}



GCError file_util_add_files_list(GtkStringList *string_list, char *dir_path, char *file_extension) {
	struct dirent *entry;

	DIR *p_dir = opendir(dir_path);

	if (!p_dir) {
		return ErrorFileAccessFailed;
	}


	StrView extension;
	StrView no_extension;


	char buffer[100];

	while ((entry = readdir(p_dir)) != NULL) {
		str_view_init(&extension, entry->d_name);
		no_extension = extension;

		str_view_get_extension(&extension);
		str_view_strip_extension(&no_extension);


		if (str_view_cmp(&extension, file_extension) == 0) {
			GCError err = str_view_to_buffer(&no_extension, buffer, sizeof(buffer));

			if (err) {
				continue;
			}

			gtk_string_list_append(string_list, buffer);
		}

		
	}

	return ErrorClear;
}
