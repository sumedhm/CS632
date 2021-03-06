#include "countweekdays.h"

int *no_leap_years_1_svc(date *dt, struct svc_req * rqstp){
    int day = dt->dd, month = dt->mm;
    long int year = dt->yy;
    int leap_days = (year-1)/4 - (year-1)/100 + (year-1)/400 - 4;
    
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
    static int result;
    result = leap_days;
    return &result;
}
   
long int *no_of_days_1_svc(dates *dt, struct svc_req * rqstp){
    int day = dt->d1.dd, month = dt->d1.mm;
    long int year = dt->d1.yy;
    int leap_days = dt->arg;
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
    static long int result;
    result = days;
    return &result;
}
   
int *get_day_1_svc(dates *dt, struct svc_req * rqstp){
    long int days = dt->arg;
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

long int *get_no_of_days_1_svc(dates *d, struct svc_req * rqstp){
    date *d1 = &(d->d1);
    date *d2 = &(d->d2);
    long int days1 = *no_of_days_1_svc(d1,rqstp);
    long int days2 = *no_of_days_1_svc(d2,rqstp);
    static long int result;
    if(days2<days1) result = (days1-days2);
    else result = (days2-days1);
    return &result;
}

long int *get_no_of_weekday_1_svc(dates *d, struct svc_req * rqstp){
    int *given_day = &(d->arg);
    date *d1 = &(d->d1);
    date *d2 = &(d->d2);
    int day1 = *get_day_1_svc(d1,rqstp);
    int day2 = *get_day_1_svc(d2,rqstp);
    int week1, week2;
    static long int weeks;
    long int *days = get_no_of_days_1_svc(d,rqstp);
    if(*days<7){
	weeks = 0;
        if(day1<=*given_day && day2>=*given_day) weeks = 1;
        return (&weeks);
    }
    if(day1<=*given_day) week1 = 1;
    else week1 = 0;
    if(day2>=*given_day) week2 = 1;
    else week2 = 0;
    *days -= (8 - day1);
    *days -= (day2-1);
    weeks = *days/7 + week1 + week2;
    return (&weeks);
}

