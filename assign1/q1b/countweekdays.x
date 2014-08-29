#define VERSION_NUMBER 1

struct date{
	int dd;
	int mm;
	long int yy;
};

struct dates{
	date d1;
	date d2;
	int arg;
};

program COUNT_DAYS{
	version COUNT_DAYS_VERSION{
		int no_leap_years(date) = 1;
		long int no_of_days(date) = 2;
		int get_day(date) = 3;
		long int get_no_of_days(dates) = 4;
		long int get_no_of_weekday(dates) = 5;
	} = 1;
} = 0x3987642;
