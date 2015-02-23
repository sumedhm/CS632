#include "coda.h"

CLIENT *clnt;
int  *result_1;
char * arg;

int
check_connection(){
	result_1 = check_connection_1(result_1, clnt);
	if(result_1 == (int *) NULL){
		return 0;
	}
	return 1;
}

int
get_log(char *arg){
	result_1 = get_log_1(arg, clnt);
	if(result_1 == (int *)NULL){
		clnt_perror(clnt, "Getting log url from server failed..\n");
		return 0;
	}
	
	return 1;
}

void
coda_1(char *host)
{
	arg = (char *) malloc (100*sizeof(char));
	sprintf(arg, "abc.txt");
	result_1 = update_file_1(&arg, clnt);
	if(result_1 == (int *) NULL) {
		clnt_perror (clnt, "Call failed.\n");
	}
	clnt_destroy (clnt);
}

int
main (int argc, char *argv[])
{
	char *host;

	if (argc < 2) {
		printf ("usage: %s server_host\n", argv[0]);
		exit (1);
	}
	host = argv[1];

	clnt = clnt_create (host, CODA, CODA_VERSION, "udp");
	if (clnt == NULL) {
                clnt_pcreateerror (host);
                exit (1);
        }
	coda_1 (host);
exit (0);
}
