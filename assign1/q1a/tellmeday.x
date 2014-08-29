#define VERSION_NUMBER 1

struct date{
	int dd;
	int mm;
	long int yy;
};

program TELL_ME_DAY{
	version TELL_ME_DAY_VERSION{
		int no_of_leap_days(date) = 1;
		long int no_of_days(date) = 2;
		string get_day(date) = 3;
	} = 1;
} = 0x3987632;
