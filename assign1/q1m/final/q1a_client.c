#include "tellmeday.h"

int *get_day_0(date dt, CLIENT * c1, CLIENT * c2, CLIENT * c3){
	long int *result;
	dates arg;
	arg.d1 = dt;
	arg = *no_leap_years_1(&arg, c1);
	arg = *no_of_days_2(&arg, c2);
	result = (long int *)get_day_3(&arg, c3);
	if(result==NULL){
		printf("Error in getting response from server..Please check your connection..\n");
	}
	return (int *)result;
}

char *to_weekday(int days){
    if(days==0) return "Monday";
    else if(days==1) return "Tuesday";
    else if(days==2) return "Wednesday";
    else if(days==3) return "Thursday";
    else if(days==4) return "Friday";
    else if(days==5) return "Saturday";
    else return "Sunday";
}

int main(int argc, char * argv[]){
	CLIENT *c1, *c2, *c3;
	int *result;
	if(argc!=3) {
		printf("Usage: ./daycalculator server dd/mm/yyyy\n");
		exit(1);
	}
	c1 = clnt_create(argv[1], TELL_ME_DAY1, TELL_ME_DAY_VERSION1, "udp");
	c2 = clnt_create(argv[1], TELL_ME_DAY2, TELL_ME_DAY_VERSION2, "udp");
	c3 = clnt_create(argv[1], TELL_ME_DAY3, TELL_ME_DAY_VERSION3, "udp");
	if (c1 == NULL || c2 == NULL || c3 == NULL) {
		clnt_pcreateerror(argv[1]);
		exit(1);
	}
	int i = 0;
	date dt;
	int a = (int)(argv[2][i++]-'0');
	if(argv[2][i]!='/'){
		a *= 10;
		a += (int)(argv[2][i++]-'0');
	}
	if(argv[2][i]=='/') i++;
	dt.dd = a;
	a = (int)(argv[2][i++]-'0');
        if(argv[2][i]!='/'){
                a *= 10;
                a += (int)(argv[2][i++]-'0');
        }
        if(argv[2][i]=='/') i++;
	dt.mm = a;a = 20;
        while(argv[2][i]!='\0')i++;
        while(argv[2][i]!='/') dt.yy[a--] = (int)(argv[2][i--]-'0');
	while(a>=0) dt.yy[a--] = 0;
	if(dt.dd<1||dt.dd>31||dt.mm<1||dt.mm>12){
		printf("%s - The date you entered is invalid, please try again.\n", argv[2]);
		exit(1);
	}
	printf("\nInput - %s\n",argv[2]);
	result = get_day_0(dt, c1, c2, c3);
	if (result == NULL){
		clnt_perror(c1, "Error in getting result.");
		exit(1);
	}
	printf("Output - %s\n", to_weekday(*result));
	return 0;
}
