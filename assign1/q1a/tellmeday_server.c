#include "tellmeday.h"

int *
no_of_leap_days_1_svc(date *d, struct svc_req * rqstp){
    int leap_days = (d->yy-1)/4 - (d->yy-1)/100 + (d->yy-1)/400 - 4;
    int is_leap;
    if(!(d->yy%400)){
        is_leap = 1;
    } else if(!(d->yy%100)){
        is_leap = 0;
    } else if(!(d->yy%4)){
        is_leap = 1;
    } else {
        is_leap = 0;
    }

    if(is_leap){
        if(d->mm>2) leap_days++;
        else if(d->mm==2){
            if(d->dd>28) leap_days++;
        }
    }
	static int result;
	result = leap_days;
    return &result;
}

long int *
no_of_days_1_svc(date *d, struct svc_req * rqstp){
    int day = d->dd, month = d->mm, year = d->yy;
    int leap_days = *no_of_leap_days_1_svc(d, rqstp);
    int days = leap_days + (365*(year-1));
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

char **
get_day_1_svc(date *d, struct svc_req * rqstp){
    long int days = *no_of_days_1_svc(d, rqstp);
    days -= 693592; //Calculating with reference to 1900
    days %= 7;
    static char *result;
	result = (char *) malloc (10 * sizeof(char));
    if(days==0) strcpy(result, "Monday");
    else if(days==1) strcpy(result, "Tuesday");
    else if(days==2) strcpy(result, "Wednesday");
    else if(days==3) strcpy(result, "Thursday");
    else if(days==4) strcpy(result, "Friday");
    else if(days==5) strcpy(result, "Saturday");
    else strcpy(result, "Sunday");
	return &result;
}

