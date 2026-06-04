#pragma once

#include "core/error.h"
#include <stdio.h>

const char *text_loader_load_text(char *file_path);

bool text_loader_check_for_matching_word(const char *word);
char *text_loader_advance_word();
