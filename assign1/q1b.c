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
   
int get_day(int day, int month, long long int year){
    long long int days = *no_of_days(day, month, year);
    days -= 693592; //Calculating with reference to 1900
    return (days%7);
    /*if(days==0) return "Monday";
    else if(days==1) return "Tuesday";
    else if(days==2) return "Wednesday";
    else if(days==3) return "Thursday";
    else if(days==4) return "Friday";
    else if(days==5) return "Saturday";
    else return "Sunday";*/
}

long long int get_no_of_days(date d1, date d2){
    long long int days1 = *no_of_days(d1.dd, d1.mm, d1.yy);
    long long int days2 = *no_of_days(d2.dd, d2.mm, d2.yy);
    if(days2<days1) return (days1-days2);
    return (days2-days1);
}

long long int get_no_of_weekday(date d1, date d2, int given_day){
    //Given_day = 2 = Wednesday
    int day1 = get_day(d1.dd, d1.mm, d1.yy);
    int day2 = get_day(d2.dd, d2.mm, d2.yy);
    int week1, week2;
    long long int weeks;
    
    long long int days = get_no_of_days(d1,d2);
    if(days<7){
        if(day1<=given_day && day2>=given_day) return 1;
        else return 0;
    }
    if(day1<=given_day) week1 = 1;
    else week1 = 0;
    if(day2>=given_day) week2 = 1;
    else week2 = 0;
    days -= (8 - day1);
    days -= day2;
    weeks = days/7 + week1 + week2;
    return weeks;
}

int main()
{
    date d1 = {11,8,2002}, d2={13,8,2014};
    int day = 2;
    long long int result = get_no_of_weekday(d1, d2, day);
    printf("Result - %lld", result);
    return 0;
}
