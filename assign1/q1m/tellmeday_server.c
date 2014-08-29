#include "tellmeday.h"

dates *no_leap_years_1_svc(dates *dt, struct svc_req * rqstp){
    int day = dt->d1.dd, month = dt->d1.mm;
    long int year = 0;int i=0;

    while(i<20){
	year = year*10 + dt->d1.yy[i];
	i++;
    }
    long int leap_days = (year-1)/4 - (year-1)/100 + (year-1)/400 - 4;

    int is_leap;
    
    if(!(year%400)){
        is_leap = 1;
    } else if(!(year%100)){
        is_leap = 0;
    } else if(!(year%4)){
        is_leap = 1;
    } else {
        is_leap = 0;
    }
    
    if(is_leap){
        if(month>2) leap_days++;
        else if(month==2){
            if(day>28) leap_days++;
        }
    }
    i=19;
    while(i>=0){
	dt->res[i--] = leap_days%10;
	leap_days /= 10;
    }
    return dt;
}
   
dates *no_of_days_1_svc(dates *dt, struct svc_req * rqstp){
    int day = dt->d1.dd, month = dt->d1.mm;
    long int year = 0;int i=0;
    while(i<20) year = year*10 + dt->d1.yy[i++];

    long int leap_days = 0;i=0;
    while(i<20) leap_days = leap_days*10 + dt->res[i++];

    long int days = leap_days + (365*(year-1));
    if(month>1) days+=31;
    if(month>2) days+=28;
    if(month>3) days+=31;
    if(month>4) days+=30;
    if(month>5) days+=31;
    if(month>6) days+=30;
    if(month>7) days+=31;
    if(month>8) days+=31;
    if(month>9) days+=30;
    if(month>10) days+=31;
    if(month>11) days+=30;
    days+=day;
    i=19;
    while(i>=0){
        dt->res[i--] = days%10;
        days /= 10;
    }
    return dt;
}
   
int *get_day_1_svc(dates *dt, struct svc_req * rqstp){
    long int days = 0;int i=0;
    while(i<20) days = days*10 + dt->res[i++];
    days -= 693592; //Calculating with reference to 1900
    static int result;
    result = days%7;
    return &result;
    /*if(days==0) return "Monday";
    else if(days==1) return "Tuesday";
    else if(days==2) return "Wednesday";
    else if(days==3) return "Thursday";
    else if(days==4) return "Friday";
    else if(days==5) return "Saturday";
    else return "Sunday";*/
}

dates *get_no_of_weekday_1_svc(dates *arg, struct svc_req * rqstp){
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

