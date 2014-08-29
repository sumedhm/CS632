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
