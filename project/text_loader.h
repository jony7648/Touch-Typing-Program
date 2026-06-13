#pragma once

#include "core/error.h"
#include "core/app.h"
#include "core/listener.h"
#include "core/time_componet.h"


#include <stdio.h>

const char *text_loader_load_text(char *file_path);

enum {
	TextLoaderSigIDTextLoaded,
	TextLoaderSigIDTextReloaded,
	TextLoaderSigIDTestFinished,
	TextLoaderSignalCount,	
};


typedef struct {
	const char *text;
} TextLoaderSigTextLoaded;

typedef struct {
	const char *text;
} TextLoaderSigTextReloaded;

typedef struct {
	GCTime end_time;
	size_t char_count;
} TextLoaderSigTestFinished;

void text_loader_init(GCApp*);
bool text_loader_check_for_matching_word(const char *word);
char *text_loader_advance_word();
void text_loader_listen_for(int id, GCEmitFuncPtr func);
