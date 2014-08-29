#include "tellmeday.h"
   
dates *no_of_days_2_svc(dates *dt, struct svc_req * rqstp){
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

