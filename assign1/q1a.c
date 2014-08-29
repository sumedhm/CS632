#include <stdio.h>
#include <string.h>

long long int leap_days, days;

typedef struct _date{
        int dd;
        int mm;
        long long int yy;
} date;

long long int *no_leap_years(int day, int month, long long int year){
     leap_days = (year-1)/4 - (year-1)/100 + (year-1)/400 - 4;
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
    return &leap_days;
}

long long int *no_of_days(int day, int month, long long int year){
    long long int leap_days = *no_leap_years(day, month, year);
    days = leap_days + (365*(year-1));
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
    return &days;
}

char *get_day(int day, int month, long long int year){
    long long int days = *no_of_days(day, month, year);
    days -= 693592; //Calculating with reference to 1900
    days %= 7;
    if(days==0) return "Monday";
    else if(days==1) return "Tuesday";
    else if(days==2) return "Wednesday";
    else if(days==3) return "Thursday";
    else if(days==4) return "Friday";
    else if(days==5) return "Saturday";
    else return "Sunday";
}

int main(){
    date d1 = {10,8,2014};
    printf("%s\n", get_day(d1.dd, d1.mm, d1.yy));
    return 0;
}
