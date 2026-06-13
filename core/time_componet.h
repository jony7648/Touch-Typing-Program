#pragma once

#include <stdint.h>
#include <stdio.h>

#define MTH_PER_YEAR 12
#define DAYS_PER_YEAR 365
#define DAYS_PER_WEEK 7
#define HOURS_PER_DAY 24
#define MINUTES_PER_HOUR 60
#define SECONDS_PER_MINUTE 60

typedef struct {
	int year;
	int8_t day, weekday, month;
} GCDate;

typedef struct {
	GCDate curr_date;
	GCDate menu_date;
} GCFullDate;

typedef struct {
	uint16_t hour;
	uint16_t minute;
	uint16_t second;
} GCTime;



void gctime_date_display_info(GCDate *date);
void gctime_time_display_info(GCTime *time);
void gctime_init(GCFullDate *gc_time);
void gctime_set_date_from_sys_clock(GCDate *gc_date);
void gctime_get_current_time(GCTime *time);
float gctime_get_wpm(GCTime *start_time, GCTime *end_time, size_t char_count);
