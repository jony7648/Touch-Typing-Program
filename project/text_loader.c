#include "text_loader.h"
#include "strings.h"
#include "core/listener.h"
#define TEXT_BUFF_SIZE 10000
char G_type_test_buffer[TEXT_BUFF_SIZE];
char *G_type_test_use_ptr = G_type_test_buffer;
size_t G_text_buffer_pos;
size_t G_text_len;


Listener G_listener;


const char ADVANCE_CHAR = ' ';


void text_loader_init(GCApp*) {
	listener_init(&G_listener, NULL, GCTypeEmpty, TextLoaderSignalCount);	
}



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
	store_buff[store_index + 1] = '\0';
	return store_index;
}

const char *text_loader_get_loaded_text() {
	return G_type_test_buffer;
}

const char *text_loader_load_text(char *file_path) {
	FILE *fptr = fopen(file_path, "r");

	char *ret_ptr = G_type_test_buffer;

	printf("File Path: %s\n", file_path);
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


	//get rid of UTF8 header
	if (strncmp(G_type_test_buffer, "\xEF\xBB\xBF", 3) == 0) {
		ret_ptr += 3;
	}

	G_type_test_use_ptr = ret_ptr;

	fclose(fptr);
	fptr = NULL;

	TextLoaderSigTextLoaded signal = {
		.text = ret_ptr,
	};

	listener_emit(&G_listener, TextLoaderSigIDTextLoaded, &signal);

	return ret_ptr;
}

void text_loader_restart_text() {
	G_text_buffer_pos = 0;	

	TextLoaderSigTextReloaded signal = {
		.text = G_type_test_use_ptr,	
	};

	listener_emit(&G_listener, TextLoaderSigIDTextReloaded, &signal);	
}



bool text_loader_check_for_matching_word(const char *word) {
	size_t word_len = strlen(word);

	for (size_t i = 0; i<word_len; i++) {
		size_t challenge_pos = i+G_text_buffer_pos;
		char challenge_char = G_type_test_use_ptr[challenge_pos];
		char user_char = word[i];


		//printf("pos: %c %c\n", word[i], challenge_char);
		//printf("pos: %d, %d\n", i, challenge_pos);
		//


		bool should_end_text = (challenge_char == ADVANCE_CHAR || challenge_char == '\0') && word[i] == ADVANCE_CHAR;

		if (should_end_text) {
			return true;
		}

		if (user_char != challenge_char) {
			return false;
		}
	}


	return false;
}


void test_finished(size_t char_count) {
	TextLoaderSigTestFinished signal;

	signal.char_count = char_count;

	gctime_get_current_time(&signal.end_time);

	listener_emit(&G_listener, TextLoaderSigIDTestFinished, &signal);
}

char *text_loader_advance_word() {
	size_t i = 0;

	if (G_type_test_use_ptr[0] == '\0') {
		printf("No text is loaded!");
		return NULL;
	}


	//if the end of the text was found do not look for the next word
	//and jump to the end of the function
	for (i=G_text_buffer_pos; G_type_test_use_ptr[i] != ' '; i++) {
		if (G_type_test_use_ptr[i] == '\0') {
			test_finished(i);
			return G_type_test_use_ptr;
		}
	}


	//Keep checking for the end of the text just in case the writer inserted
	//extra spaces at the end
	for (i=i; G_type_test_use_ptr[i] == ' '; i++) {
		if (G_type_test_use_ptr[i] == '\0') {
			test_finished(i);
			break;
		}
	}

end_func:



	G_text_buffer_pos = i;

	return &G_type_test_use_ptr[i];
}





void text_loader_listen_for(int id, GCEmitFuncPtr func) {
	listener_listen(&G_listener, id, func);
}
