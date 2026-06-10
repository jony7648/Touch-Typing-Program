#include "time_componet.h"
#include "core/str_view.h"
#include <stdio.h>
#include <time.h>


const char *MONTH_STR_ARR[GCTIME_MTH_PER_YEAR] = {
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

const char *DAY_STR_ARR[GCTIME_DAYS_PER_WEEK] = {
	"Mon",
	"Tue",
	"Wed",
	"Thu",
	"Fri",
	"Sat",
	"Sun",
};

const int DAY_COUNT_ARR[GCTIME_MTH_PER_YEAR] = {
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


void gctime_init(GCTime *time) {
	time->curr_date = (GCDate){
		.day = 0,
		.month = 0,
		.year = 0,
		.weekday = 0,
	};

	time->menu_date = time->curr_date;
}

void gctime_display_info(GCTime *p) {
	printf("mth: %d\n", p->curr_date.month);
	printf("weekday: %d\n", p->curr_date.weekday);
	printf("day: %d\n", p->curr_date.day);
	printf("year: %d\n", p->curr_date.year);
}

int gctime_month_str_to_index(GCTime *time, StrView *view) {
	int month = 0;

	for (int i=0; i<GCTIME_MTH_PER_YEAR; i++) {
		const char *month_str = MONTH_STR_ARR[i];

		if (str_view_cmp(view, month_str) == 0) {
			month = i;
			break;
		}
	}

	return month;
}

int gctime_weekday_str_to_index(GCTime *time, StrView *view) {
	int weekday = 0;

	for (int i=0; i<GCTIME_DAYS_PER_WEEK; i++) {
		const char *weekday_str = DAY_STR_ARR[i];

		if (str_view_cmp(view, weekday_str) == 0) {
			weekday = i;
			break;
		}
	}

	return weekday;

}

void ctime_to_gc_time(GCTime *gc_time, const char* ctime) {
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
				gc_time->curr_date.weekday = gctime_weekday_str_to_index(gc_time, &it.view);
				break;
			case Month:
				gc_time->curr_date.month = gctime_month_str_to_index(gc_time, &it.view);
				break;
			case Day:
				gc_time->curr_date.day = str_view_to_int(&it.view);
				break;
			case Time:
				break;
			case Year:
				gc_time->curr_date.year = str_view_to_int(&it.view);
				break;
		}
	}
}

void gctime_set_date_from_sys_clock(GCTime *gc_time) {
	time_t sys_time;
	sys_time = time(&sys_time);

	char time_str[100];
	time_str[0] = '\0';

	ctime_r(&sys_time, time_str);

	
	ctime_to_gc_time(gc_time, time_str);

}	
