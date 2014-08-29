#include "tellmeday.h"

dates *get_no_of_weekday_4_svc(dates *arg, struct svc_req * rqstp){
    int day1 = (int)arg->d1.yy[0];
    int day2 = (int)arg->d1.yy[1];
    int given_day = arg->d1.yy[2];
    int week1, week2,i;
    long int weeks;
    long int days = 0;
    for(i=0;i<20;i++) days=days*10+arg->res[i];
    if(days<7){
	weeks = 0;
        if(day1<=given_day || day2>=given_day) weeks = 1;
	for(i=0;i<19;i++) arg->res[i]=0;
	arg->res[19] = (int) weeks;
        return arg;
    }
    if(day1<=given_day) week1 = 1;
    else week1 = 0;
    if(day2>=given_day) week2 = 1;
    else week2 = 0;
    days -= (8 - day1);
    days -= (day2-1);
    weeks = days/7 + week1 + week2;
    i=19;
    while(i>=0){
        arg->res[i--] = weeks%10;
        weeks /= 10;
    }
    return arg;
}

