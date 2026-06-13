#include "time_componet.h"
#include "core/str_view.h"
#include <stdio.h>
#include <time.h>

#define CHARACTERS_PER_WORD 5

const char *MONTH_STR_ARR[MTH_PER_YEAR] = {
	"Jan",
	"Feb",
	"Mar",
	"Apr",
	"May",
	"Jun",
	"Jul",
	"Aug",
	"Sep",
	"Oct",
	"Nov",
	"Dec",
};

const char *DAY_STR_ARR[DAYS_PER_WEEK] = {
	"Mon",
	"Tue",
	"Wed",
	"Thu",
	"Fri",
	"Sat",
	"Sun",
};

const int DAY_COUNT_ARR[MTH_PER_YEAR] = {
	31,
	28,
	31,
	30,
	31,
	30,
	31,
	31,
	30,
	31,
	30,
	31,
};


void gctime_init(GCFullDate *time) {
	time->curr_date = (GCDate){
		.day = 0,
		.month = 0,
		.year = 0,
		.weekday = 0,
	};

	time->menu_date = time->curr_date;
}

void gctime_date_display_info(GCDate *p) {
	printf("mth: %d\n", p->month);
	printf("weekday: %d\n", p->weekday);
	printf("day: %d\n", p->day);
	printf("year: %d\n", p->year);
}

void gctime_time_display_info(GCTime *p) {
	printf("Hour: %d\n", p->hour);
	printf("Minute: %d\n", p->minute);
	printf("Second: %d\n", p->second);
}

int gctime_month_str_to_index(GCDate *time, StrView *view) {
	int month = 0;

	for (int i=0; i<MTH_PER_YEAR; i++) {
		const char *month_str = MONTH_STR_ARR[i];

		if (str_view_cmp(view, month_str) == 0) {
			month = i;
			break;
		}
	}

	return month;
}

int gctime_weekday_str_to_index(GCDate *time, StrView *view) {
	int weekday = 0;

	for (int i=0; i<DAYS_PER_WEEK; i++) {
		const char *weekday_str = DAY_STR_ARR[i];

		if (str_view_cmp(view, weekday_str) == 0) {
			weekday = i;
			break;
		}
	}

	return weekday;

}

void ctime_to_gc_time(GCDate *gc_date, const char* ctime) {
	const int EXPECTED_COUNT = 5;
	int weekday_len = 3;
	int month_len = 3;
	int day_len = 2;


	StrView full_view;
	str_view_init(&full_view, ctime);

	StrViewIterator it;
	str_view_iterator_init(&it, &full_view);

	enum {Weekday, Month, Day, Time, Year};

	for (int i=0; str_view_iterator_has_next(&it); i++) {
		switch (i) {
			case Weekday:
				gc_date->weekday = gctime_weekday_str_to_index(gc_date, &it.view);
				break;
			case Month:
				gc_date->month = gctime_month_str_to_index(gc_date, &it.view);
				break;
			case Day:
				gc_date->day = str_view_to_int(&it.view);
				break;
			case Time:
				break;
			case Year:
				gc_date->year = str_view_to_int(&it.view);
				break;
		}
	}
}

void gctime_set_date_from_sys_clock(GCDate *gc_date) {
	time_t sys_time;
	sys_time = time(&sys_time);

	char time_str[100];
	time_str[0] = '\0';

	ctime_r(&sys_time, time_str);

	
	ctime_to_gc_time(gc_date, time_str);

}	

void gctime_get_current_time(GCTime *gc_time) {
	const int TIME_SECTION_START = 11;
	const int TIME_SECTION_LEN = 8;

	enum {Hour, Minute, Second};

	time_t sys_time;
	sys_time = time(&sys_time);

	char time_buffer[100];
	ctime_r(&sys_time, time_buffer);

	StrView view_arr[3];
	
	Arena arena;
	arena_init(&arena, view_arr, sizeof(view_arr));

	StrView src_view = {
		.ptr = time_buffer + TIME_SECTION_START,
		.len = TIME_SECTION_LEN,
	};

	size_t view_count = str_view_split(&src_view, &arena, ':', true);

	//get the time from the split views
	for (int i=0; i<view_count; i++) {
		StrView *view = &view_arr[i];

		int num = str_view_to_int(view);
		str_view_display(view);

		switch (i) {
			case Hour:
				gc_time->hour = num;
				break;
			case Minute:
				gc_time->minute = num;
				break;
			case Second:
				gc_time->second = num;
				break;
		}
	}

	printf("%s\n", time_buffer);

}

float gctime_get_wpm(GCTime *t_start, GCTime *t_end, size_t char_count) {
	int hour_diff = t_end->hour - t_start->hour;
	int minute_diff = t_end->minute - t_start->minute;
	int second_diff = t_end->second - t_start->second;

	if (hour_diff < 0) {
		hour_diff += HOURS_PER_DAY;
	}

	if (minute_diff < 0) {
		minute_diff += MINUTES_PER_HOUR;
		hour_diff--;
	}

	if (second_diff < 0) {
		second_diff += SECONDS_PER_MINUTE;
		minute_diff--;
	}


	float elaspsed_minutes = 
		MINUTES_PER_HOUR*hour_diff +
		minute_diff +
		(float)second_diff / SECONDS_PER_MINUTE;

	float cpm = char_count/elaspsed_minutes;

	float wpm = cpm/CHARACTERS_PER_WORD;

	printf("Elapsed: %f\n",wpm);
	printf("Char count: %d\n", char_count);

	return wpm;

}
