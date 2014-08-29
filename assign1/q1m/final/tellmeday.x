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

program TELL_ME_DAY1{
	version TELL_ME_DAY_VERSION1{
		dates no_leap_years(dates) = 1;
	} = 1;
} =0x1000001;
program TELL_ME_DAY2{
	version TELL_ME_DAY_VERSION2{
		dates no_of_days(dates) = 2;
	} = 2;
}=0x1000002;
program TELL_ME_DAY3{
	version TELL_ME_DAY_VERSION3{
		int get_day(dates) = 3;
	} = 3;
}=0x1000003;
program TELL_ME_DAY4{
	version TELL_ME_DAY_VERSION4{
		dates get_no_of_weekday(dates) = 4;
	} = 4;
}=0x1000004;

