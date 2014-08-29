#include "tellmeday.h"

long int get_day_0(dates *d, dates *dx, int given, CLIENT * clnt){
	long int *result;
	int i=0;
	long int days1=0, days2=0;
	d = no_leap_years_1(d, clnt);
        d = no_of_days_1(d, clnt);
	for(i=0;i<20;i++) days1 = days1*10 + d->res[i];
        result = (long int *)get_day_1(d, clnt);
        if(result==NULL){
                printf("Error in getting response from server..Please check your connection..\n");
        }
	dates *args = (dates *)malloc(sizeof(dates));
        args->d1.yy[0] = (int) *result;  //day1
	dx = no_leap_years_1(dx, clnt);
        dx = no_of_days_1(dx, clnt);
	for(i=0;i<20;i++) days2 = days2*10 + dx->res[i];
        result = (long int *)get_day_1(dx, clnt);
        if(result==NULL){
                printf("Error in getting response from server..Please check your connection..\n");
        }
        args->d1.yy[1] = (int) *result; //day2
	long int result1;
	if(days1<=days2){
		result1 = days2-days1;
	} else {
		result1 = days1-days2;
		int temp = args->d1.yy[0];
		args->d1.yy[0] = args->d1.yy[1];
		args->d1.yy[1] = temp;
	}
	args->d1.yy[2] = given;
	for(i=19;i>=0;i--){
		args->res[i] = result1%10;
		result1 /= 10;
	}
	args = get_no_of_weekday_1(args, clnt);
	if(args==(dates *)NULL){
		printf("Error in getting response from server\n");
	}
	result1 = 0;
	for(i=0;i<20;i++) result1 = result1*10 + args->res[i];
	return result1;
}

int get_weekday(char *day){
	if(!strcmp(day, "Monday")) return 0;
	if(!strcmp(day, "Tuesday")) return 1;
	if(!strcmp(day, "Wednesday")) return 2;
	if(!strcmp(day, "Thursday")) return 3;
	if(!strcmp(day, "Friday")) return 4;
	if(!strcmp(day, "Saturday")) return 5;
	if(!strcmp(day, "Sunday")) return 6;
	return -1;
}

int main(int argc, char * argv[]){
	CLIENT *cl;
	long int *result;
	if(argc!=5) {
		printf("Usage: ./q2a server date1 date2 day\n");
		exit(1);
	}
	cl = clnt_create(argv[1], TELL_ME_DAY, TELL_ME_DAY_VERSION, "udp");
	if (cl == NULL) {
		clnt_pcreateerror(argv[1]);
		exit(1);
	}
	int i = 0;
	date d1, d2;
	int a = (int)(argv[2][i++]-'0');
	if(argv[2][i]!='/'){
		a *= 10;
		a += (int)(argv[2][i++]-'0');
	}
	if(argv[2][i]=='/') i++;
	d1.dd = a;
	a = (int)(argv[2][i++]-'0');
        if(argv[2][i]!='/'){
                a *= 10;
                a += (int)(argv[2][i++]-'0');
        }
        if(argv[2][i]=='/') i++;
	d1.mm = a;
	a = 20;
        while(argv[2][i]!='\0')i++;
        while(argv[2][i]!='/') d1.yy[a--] = (int)(argv[2][i--]-'0');
        while(a>=0) d1.yy[a--] = 0;
	i = 0;
	a = (int)(argv[3][i++]-'0');
        if(argv[3][i]!='/'){
                a *= 10;
                a += (int)(argv[3][i++]-'0');
        }
        if(argv[3][i]=='/') i++;
        d2.dd = a;
        a = (int)(argv[3][i++]-'0');
        if(argv[3][i]!='/'){
                a *= 10;
                a += (int)(argv[3][i++]-'0');
        }
        if(argv[3][i]=='/') i++;
        d2.mm = a;
	a = 20;
        while(argv[3][i]!='\0')i++;
        while(argv[3][i]!='/') d2.yy[a--] = (int)(argv[3][i--]-'0');
        while(a>=0) d2.yy[a--] = 0;

	if(d1.dd<1||d1.dd>31||d1.mm<1||d1.mm>12){
		printf("%s- The date you entered is invalid, please try again.\n", argv[2]);
		exit(1);
	}
	if(d2.dd<1||d2.dd>31||d2.mm<1||d2.mm>12){
                printf("%s- The date you entered is invalid, please try again.\n", argv[3]);
                exit(1);
        }
	int day = get_weekday(argv[4]);
	if(day==-1){
		printf("%s - The day you entered is invalid, please try again.\n", argv[4]);
                exit(1);
	}
	dates dt1;dt1.d1 = d1;
	dates dt2;dt2.d1 = d2;
	printf("\nInput - %s, %s, %s\n", argv[2], argv[3], argv[4]);
	long int ans;
	ans = get_day_0(&dt1, &dt2, day, cl);
	/*if (result == (long int *)NULL){
		clnt_perror(cl, "Error in getting result.");
		exit(1);
	}*/
	printf("Output - %ld\n", ans);
	return 0;
}
