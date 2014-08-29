#include "tellmeday.h"

char *get_day_0(date *dt, CLIENT * clnt){
	char *result;
	result = (char *) malloc (10*sizeof(char));
	result = *get_day_1(dt, clnt);
	if(result==(char *)NULL){
		printf("Error in getting response from server\n");
	}
	return result;
}

int main(int argc, char * argv[]){
	CLIENT *cl;
	char *result;
	result = (char *) malloc (10*sizeof(char));
	if(argc!=3) {
		printf("Usage: ./daycalculator server dd/mm/yyyy\n");
		exit(1);
	}
	cl = clnt_create(argv[1], TELL_ME_DAY, TELL_ME_DAY_VERSION, "udp");
	if (cl == NULL) {
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
	dt.mm = a;
	a = (int)(argv[2][i++]-'0');
        while(argv[2][i]!='\0'){
                a *= 10;
                a += (int)(argv[2][i++]-'0');
        }
        dt.yy = a;

	if(dt.dd<1||dt.dd>31||dt.mm<1||dt.mm>12||dt.yy<1){
		printf("%d/%d/%ld- The date you entered is invalid, please try again.\n", dt.dd, dt.mm, dt.yy);
		exit(1);
	}
	printf("\nInput - %d/%d/%ld\n", dt.dd, dt.mm, dt.yy);
	result = get_day_0(&dt, cl);
	if (result == NULL){
		clnt_perror(cl, "Error in getting result.");
		exit(1);
	}

	printf("Output - %s\n", *result);
	return 0;
}
