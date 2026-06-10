#pragma once

#include <stdint.h>

#define GCTIME_MTH_PER_YEAR 12
#define GCTIME_DAYS_PER_YEAR 365
#define GCTIME_DAYS_PER_WEEK 7



typedef struct {
	int year;
	int8_t day, weekday, month;
} GCDate;

typedef struct {
	GCDate curr_date;
	GCDate menu_date;
} GCTime;

void gctime_init(GCTime *gc_time);
void gctime_set_date_from_sys_clock(GCTime *gc_time);
