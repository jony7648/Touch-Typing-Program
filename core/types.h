#pragma once

#include <stdint.h>
#include <stdio.h>

typedef void (*VoidFuncPtr)(int);

typedef uint8_t byte;


typedef struct {
	uint8_t size;
	char data[5];
} IChar;

void ichar_display(IChar ichar);
