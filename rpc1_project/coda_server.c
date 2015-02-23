#include "coda.h"
#include "string.h"
#define LEADER 1
#define ROLE 0
#define num 3
#define myip "127.0.0.1"

char *servers[num];

void
init_servers()
{
	int i;
	for(i=0;i<num;i++){
		servers[i] = (char *) malloc (20*sizeof(char));
	}
	strcpy(servers[0],"127.0.0.1");
	strcpy(servers[1],"127.0.0.2");
	strcpy(servers[2],"127.0.0.3");
}

void
get_log(char *filename)
{

}

void 
get_log_for_dir(const char *name, int level, int server)
{
    DIR *dir;
    struct dirent *entry;

    if (!(dir = opendir(name)))
        return;
    if (!(entry = readdir(dir)))
        return;

    do {
        if (entry->d_type == DT_DIR) {
            char path[1024];
            int len = snprintf(path, sizeof(path)-1, "%s/%s", name, entry->d_name);
            path[len] = 0;
            if (strcmp(entry->d_name, ".") == 0 || strcmp(entry->d_name, "..") == 0)
                continue;
            //printf("%*s[%s]\n", level*2, "", entry->d_name);
            get_log_for_dir(path, level + 1);
        }
        else {
	    char file[1024];
            sprintf(file, "%s/%s", name, entry->d_name);
	    get_log(file, server);
	    update_file(file, server);
	}
    } while (entry = readdir(dir));
    closedir(dir);
}

int *
sync_servers(){

	init_servers();
	int i;
	for(i=0;i<num;i++){
		//get logs
		get_log_for_dir(".", 0, i);
	}
	for(i=0;i<num;i++){
		
	}
}

int *
check_connection_1_svc(int *a, struct svc_req *rqstp)
{
	static int res;
	res = 1;
	return &res;
}

int *
update_file_1_svc(char **argp, struct svc_req *rqstp)
{
	static int  result;
	char *log;
	log = (char *) malloc (104*sizeof(char));
	strcpy(log,*argp);
	sprintf(*argp, "/srv/ftp/%s", log);
	FILE *fp = fopen(*argp,"a+");
	if(fp==NULL){
		printf("Error opening file %s..\n", *argp);
	}
	printf("Will be updating %s..\n", *argp);
	char buffer[256];
	sprintf(log, "%s.log", *argp);
	FILE *logFile = fopen(log, "r");
	if(logFile==NULL){
		printf("Error opening file.\n");
	}
	else {
		while(fgets(buffer, sizeof(buffer), logFile)) {
			fprintf(fp, "%s", buffer);
		}
	}
	fclose(logFile);
	fclose(fp);

	return &result;
}
