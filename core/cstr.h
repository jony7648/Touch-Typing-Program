#pragma once

#include "error.h"
#include <stdio.h>

void cstr_from_float(char buffer[], float num, int decimals);
GCError cstr_combine(char *buffer, size_t buffer_size, char *str_arr[]); //ENSURE THAT NULL IS AT THE END TO MARK THE ENDING!!

																			  
