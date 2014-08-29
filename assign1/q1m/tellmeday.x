#define VERSION_NUMBER 1

struct date{
	int dd;
	int mm;
	int yy[20];
};

struct dates{
	date d1;
	int res[20];
};

program TELL_ME_DAY{
	version TELL_ME_DAY_VERSION{
		dates no_leap_years(dates) = 1;
		dates no_of_days(dates) = 2;
		int get_day(dates) = 3;
		dates get_no_of_weekday(dates) = 4;
	} = 1;
} = 0x3987642;
