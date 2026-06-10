#pragma once

#include "core/error.h"
#include "core/app.h"
#include <stdio.h>
#include "core/listener.h"

const char *text_loader_load_text(char *file_path);

enum {
	TextLoaderSigIDTextLoaded,
	TextLoaderSigIDTextReloaded,
	TextLoaderSignalCount,	
};


typedef struct {
	const char *text;
} TextLoaderSigTextLoaded;

typedef struct {
	const char *text;
} TextLoaderSigTextReloaded;

void text_loader_init(GCApp*);
bool text_loader_check_for_matching_word(const char *word);
char *text_loader_advance_word();
void text_loader_listen_for(int id, GCEmitFuncPtr func);
