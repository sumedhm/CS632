#include "tellmeday.h"
   
int *get_day_3_svc(dates *dt, struct svc_req * rqstp){
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

