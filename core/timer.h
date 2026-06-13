#pragma once

#include "error.h"

#include <stdio.h>
#include <stdint.h>

typedef struct {
} TimerSignalTimeout;

GCError timer_spawn(uint32_t seconds, void (timeout_func)(TimerSignalTimeout*));
