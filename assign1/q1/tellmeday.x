#define VERSION_NUMBER 1

struct date{
	int dd;
	int mm;
	long int yy;
};

struct dates{
	date d1;
	date d2;
	long int arg;
};

struct args{
	int d1;
	int d2;
	int d;
	long int days;
};

program TELL_ME_DAY{
	version TELL_ME_DAY_VERSION{
		int no_leap_years(date) = 1;
		long int no_of_days(dates) = 2;
		int get_day(dates) = 3;
		long int get_no_of_weekday(args) = 4;
	} = 1;
} = 0x3987642;
