#include "text_loader.h"
#include "strings.h"
#define TEXT_BUFF_SIZE 10000
char G_type_test_buffer[TEXT_BUFF_SIZE];
char *G_type_test_use_ptr = G_type_test_buffer;
size_t G_text_buffer_pos = 0;


const char ADVANCE_CHAR = ' ';

size_t buffer_append(char *store_buff, char *copy_buff, size_t buffer_start) {
	size_t i = 0;
	size_t store_index = buffer_start + i;
	//printf("in: %d\n", store_index);

	for (i=0; copy_buff[i] != '\0' && copy_buff[i] != '\n'; i++) {
		store_index = buffer_start + i;

		if (copy_buff[i] == '\r') {
			continue;
		}

		if (store_index >= TEXT_BUFF_SIZE) {
			store_buff[TEXT_BUFF_SIZE - 1] = '\0';
			return TEXT_BUFF_SIZE;
		}

		store_buff[store_index] = copy_buff[i];
	}


	//printf("%s", store_buff);
	store_buff[store_index] = '\0';
	return store_index;
}

const char *text_loader_get_loaded_text() {
	return G_type_test_buffer;
}

const char *text_loader_load_text(char *file_path) {
	FILE *fptr = fopen(file_path, "r");

	char *ret_ptr = G_type_test_buffer;

	if (!fptr) {
		fclose(fptr);
		return NULL;		
	}
	
	G_type_test_buffer[0] = '\0';

	char buffer[500];

	size_t index = 0;
	
	while (fgets(buffer, sizeof(buffer), fptr)) {
		index = buffer_append(G_type_test_buffer, buffer, index);

		//printf("out: %d\n", index);
	}


	//get rid of UTF8 char
	if (strncmp(G_type_test_buffer, "\xEF\xBB\xBF", 3) == 0) {
		ret_ptr += 3;
	}

	G_type_test_use_ptr = ret_ptr;

	fclose(fptr);
	fptr = NULL;

	return ret_ptr;
}


bool text_loader_check_for_matching_word(const char *word) {
	size_t word_len = strlen(word);

	for (size_t i = 0; i<word_len; i++) {
		size_t challenge_pos = i+G_text_buffer_pos;
		char challenge_char = G_type_test_use_ptr[challenge_pos];


		//printf("pos: %c %c\n", word[i], challenge_char);
		//printf("pos: %d, %d\n", i, challenge_pos);
		//


		if (challenge_char == ADVANCE_CHAR) {
			return true;
		}

		if (word[i] != challenge_char) {
			return false;
		}
	}

	puts("");

	return false;
}

char *text_loader_advance_word() {
	size_t i = 0;

	for (i=G_text_buffer_pos; G_type_test_use_ptr[i] != ' '; i++);
	for (i; G_type_test_use_ptr[i] == ' '; i++);


	G_text_buffer_pos = i;

	return &G_type_test_use_ptr[i];
}
