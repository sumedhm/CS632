#include "tellmeday.h"

long int *get_day_0(dates *d, CLIENT * clnt){
	long int *result;
	int i=0;
	long int days1, days2;
	args *a;
	a = (args *) malloc(sizeof(args));
	a->d = d->arg;
	result = (long int *)no_leap_years_1(&(d->d1), clnt);
        dates arg = {d->d1,d->d1,*result};
        result = no_of_days_1(&arg, clnt);
	days1 = *result;
        arg.d1=d->d1;arg.d2=d->d1;arg.arg=*result;
        result = (long int *)get_day_1(&arg, clnt);
        if(result==NULL){
                printf("Error in getting response from server..Please check your connection..\n");
        }
        a->d1 = (int)*result;
	result = (long int *)no_leap_years_1(&(d->d2), clnt);
        arg.d1=d->d2;arg.d2=d->d2;arg.arg=*result;
        result = no_of_days_1(&arg, clnt);
        days2 = *result;
        arg.d1=d->d2;arg.d2=d->d2;arg.arg=*result;
        result = (long int *)get_day_1(&arg, clnt);
        if(result==NULL){
                printf("Error in getting response from server..Please check your connection..\n");
        }
        a->d2 = (int)*result;
	if(days1<=days2){
		a->days = days2-days1;
	} else {
		a->days = days1-days2;
		int temp = a->d1;
		a->d1 = a->d2;
		a->d2 = temp;
	}
	result = get_no_of_weekday_1(a, clnt);
	if(result==(long int *)NULL){
		printf("Error in getting response from server\n");
	}
	return result;
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
	a = (int)(argv[2][i++]-'0');
        while(argv[2][i]!='\0'){
                a *= 10;
                a += (int)(argv[2][i++]-'0');
        }
        d1.yy = a;
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
        a = (int)(argv[3][i++]-'0');
        while(argv[3][i]!='\0'){
                a *= 10;
                a += (int)(argv[3][i++]-'0');
        }
        d2.yy = a;


	if(d1.dd<1||d1.dd>31||d1.mm<1||d1.mm>12||d1.yy<1){
		printf("%d/%d/%ld- The date you entered is invalid, please try again.\n", d1.dd, d1.mm, d1.yy);
		exit(1);
	}
	if(d2.dd<1||d2.dd>31||d2.mm<1||d2.mm>12||d2.yy<1){
                printf("%d/%d/%ld- The date you entered is invalid, please try again.\n", d2.dd, d2.mm, d2.yy);
                exit(1);
        }
	int day = get_weekday(argv[4]);
	if(day==-1){
		printf("%s - The day you entered is invalid, please try again.\n", argv[4]);
                exit(1);
	}
	dates dt = {d1, d2, day};
	printf("\nInput - %d/%d/%ld, %d/%d/%ld, %s\n", d1.dd, d1.mm, d1.yy, d2.dd, d2.mm, d2.yy, argv[4]);
	result = get_day_0(&dt, cl);
	if (result == (long int *)NULL){
		clnt_perror(cl, "Error in getting result.");
		exit(1);
	}
	printf("Output - %ld\n", *result);
	return 0;
}
