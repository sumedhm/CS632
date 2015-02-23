#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h> 
#include <sys/socket.h>
#include <netinet/in.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <pthread.h>
#include <time.h>
#include <dirent.h>
#define LEADER 1
#define SERVER "172.27.30.194"
#define MAX 3
#define ID 1
#define PORT1 5001
#define PORT2 5002
#define num 2
#define LENGTH 1024
#define FTPDIR "/home/optimus/Acads/ds/sock_project/server/ftp/codafs/"
#define LOGDIR "/home/optimus/Acads/ds/sock_project/server/logs/"
#define SYNCDIR "/home/optimus/Acads/ds/sock_project/server/ftp/synclogs/"
#define SERVERLOGS "/home/optimus/Acads/ds/sock_project/server/ftp/logs/"


struct argument{
	int sock;
	char str1[1024];
	char str2[1024];
	char str3[1024];
};
typedef struct argument argument;

pthread_t p;
int sockfd, newsockfd, portno, neighbor_up, neighbor_index;
struct sockaddr_in serv_addr, clnt_addr;
char buffer[1024];
int n;
char *servers[num];
char myip[] = "172.25.0.1";

void
init_servers()
{
	int i;
	char logfile[100];
	for(i=0;i<num;i++){
		servers[i] = (char *) malloc (20*sizeof(char));
		if(strcmp(servers[i],SERVER)==0) continue;
		sprintf(logfile, "logs/servers/%s", servers[i]);
		mkdir(logfile, 0777);
	}
	strcpy(servers[0],"172.27.30.196");
	//strcpy(servers[1],"127.0.0.2");
	//strcpy(servers[2],"127.0.0.3");
	strcpy(servers[1],"172.27.30.194");
	
}

void
sync_file(char *filename){

	FILE* logfile;
	FILE* logff;
	char ffname[100];
	char fname[100];
	char logfilename[100];
	char logffilename1[100];
	char logffilename2[100];
	strcpy(ffname,FTPDIR);
	strcpy(logffilename1,SYNCDIR);
	strcpy(logffilename2,SYNCDIR);
	int len = strlen(filename);
	int loop;
	for(loop=0;loop<len-4;loop++){
		fname[loop]=filename[loop];
		if(fname[loop]=='@'){
			fname[loop]='/';
		}
		logfilename[loop]=filename[loop];
	}
	logfilename[len-4]='.';logfilename[len-3]='t';logfilename[len-2]='x';logfilename[len-1]='t';logfilename[len]='\0';
	fname[loop]='\0';
	strcat(logffilename1,logfilename);	//.txt file
	strcat(logffilename2,filename);		//.log file
	strcat(ffname,fname);
	//printf("FTP file to be updated %s\n using the following two files %s &&&& %s \n",ffname,logffilename1,logffilename2);
	loop = 0;	//FILE* fd;
	char aPath[100];// = malloc(sizeof(char) * 254);

	size_t size;
	off_t offset;
	logfile = fopen(logffilename1, "r+");
	logff = fopen(logffilename2, "r+");
	if(logfile==NULL)printf("Error in opening example log..\n");
	if(logff==NULL)printf("Error in opening log log.\n");
	//printf("Opening the files\n");
	while(fscanf(logff, "%s %zu %zu", aPath, &size, &offset)==3){
		//printf("entry made in file %s of size %d at offset %d\n", aPath, size, offset);
		char ident[2];
		char *buf = malloc(sizeof(char) * size);
		//char *puff = malloc(sizeof(char) * ((int)100));

		//char buf[] = "Winter is Coming";
		fread(buf, size, 1, logfile);
		FILE* fd;
		if(offset==0){ 
			fd = fopen(ffname, "w");
			fclose(fd);
		}

		fd = fopen(ffname, "r+");
		if(fd==NULL)printf("Couldn't open file\n");
		fseek(fd, offset, SEEK_SET);

		int res = fwrite(buf, 1, size, fd);
		//int res = pwrite(fd, buf, size, offset);
		//sleep(2);
		if(res == -1){
			printf("fwrite failed\n");
		}
		printf("No of bytes written :: %d\n",res);
		fclose(fd);
		loop++;
	}
	fclose(logfile);
	fclose(logff);

}

void
update_file(char *filename, char *server){

	FILE* logfile;
	FILE* logff;
	char ffname[100];
	char fname[100];
	char logfilename[100];
	char logffilename1[100];
	char logffilename2[100];
	char cmd[200];
	strcpy(ffname,FTPDIR);
	strcpy(logffilename1,LOGDIR);
	strcat(logffilename1,server);
	strcat(logffilename1,"/");
	strcpy(logffilename2,LOGDIR);
	strcat(logffilename2,server);
	strcat(logffilename2,"/");
	int len = strlen(filename);
	int loop;
	for(loop=0;loop<len-4;loop++){
		fname[loop]=filename[loop];
		if(fname[loop]=='@'){
			fname[loop]='/';
		}
		logfilename[loop]=filename[loop];
	}
	logfilename[len-4]='.';logfilename[len-3]='t';logfilename[len-2]='x';logfilename[len-1]='t';logfilename[len]='\0';
	fname[loop]='\0';
	strcat(logffilename1,logfilename);
	strcat(logffilename2,filename);
	strcat(ffname,fname);
	//printf("FTP file to be updated %s\n using the following two files %s &&&& %s \n",ffname,logffilename1,logffilename2);
	loop = 0;	//FILE* fd;
	char aPath[100];// = malloc(sizeof(char) * 254);

	size_t size;
	off_t offset;
	logfile = fopen(logffilename1, "r+");
	logff = fopen(logffilename2, "r+");
	if(logfile==NULL)printf("Error in opening example log..\n");
	if(logff==NULL)printf("Error in opening log log.\n");
	sprintf(cmd, "cat %s >> "SERVERLOGS"%s", logffilename2, filename);
	system(cmd);
	len = strlen(filename);
	filename[len-4] = '\0';
	strcat(filename, ".txt");
	sprintf(cmd, "cat %s >> "SERVERLOGS"%s", logffilename1, filename);
	system(cmd);
	//printf("Opening the files\n");
	while(fscanf(logff, "%s %zu %zu", aPath, &size, &offset)==3){
		//printf("entry made in file %s of size %d at offset %d\n", aPath, size, offset);
		char ident[2];
		char *buf = malloc(sizeof(char) * size);
		fread(buf, size, 1, logfile);
		fread(ident, 3, 1, logfile);
		FILE* fd;
		if(offset==0){ 
			fd = fopen(ffname, "w");
			fclose(fd);
		}

		fd = fopen(ffname, "r+");
		if(fd==NULL)printf("Couldn't open file\n");
		fseek(fd, offset, SEEK_SET);

		int res = fwrite(buf, 1, size, fd);
		if(res == -1){
			printf("fwrite failed\n");
		}
		printf("No of bytes written :: %d\n",res);
		fclose(fd);
		loop++;
	}
	fclose(logfile);
	fclose(logff);

}

void error(const char *msg)
{
	perror(msg);
	exit(1);
}

int sendall(int s, char *buf, int *len)
{
	int total = 0;        // how many bytes we've sent
	int bytesleft = *len; // how many we have left to send
	int n;

	while(total < *len) {
		n = send(s, buf+total, bytesleft, 0);
		if (n == -1) { break; }
		total += n;
		bytesleft -= n;
	}

	*len = total; // return number actually sent here

	return n==-1?-1:0; // return -1 on failure, 0 on success
} 

int send_file(char *fs_name, int sockfd){
	char sdbuf[LENGTH]; 
	printf("[Client] Sending %s to the Server... ", fs_name);
	FILE *fs = fopen(fs_name, "r");
	if(fs == NULL){
		printf("ERROR: File %s not found.\n", fs_name);
		exit(1);
	}

	bzero(sdbuf, LENGTH); 
	int fs_block_sz; 
	while((fs_block_sz = fread(sdbuf, sizeof(char), LENGTH, fs)) > 0){
		if(sendall(sockfd, sdbuf, &fs_block_sz) < 0){
			fprintf(stderr, "ERROR: Failed to send file %s. (errno = %d)\n", fs_name, errno);
			return 0;
		}
		bzero(sdbuf, LENGTH);
	}
	return 1;
}

int receive(char *fr_name, int sockfd){
	char revbuf[LENGTH];
	FILE *fr = fopen(fr_name, "w");
	if(fr == NULL)
		printf("File %s Cannot be opened.\n", fr_name);
	else{
		bzero(revbuf, LENGTH); 
		int fr_block_sz = 0;
		while((fr_block_sz = recv(sockfd, revbuf, LENGTH, 0)) > 0){
			int write_sz = fwrite(revbuf, sizeof(char), fr_block_sz, fr);
			if(write_sz < fr_block_sz){
				error("File write failed.\n");
			}
			bzero(revbuf, LENGTH);
			if (fr_block_sz == 0 || fr_block_sz != 512){
				break;
			}
		}
		if(fr_block_sz < 0){
			if (errno == EAGAIN){
				printf("recv() timed out.\n");
			}
			else{
				fprintf(stderr, "recv() failed due to errno = %d\n", errno);
			}
		}
	}
	printf("Ok received from server!\n");
	fclose(fr);
	return 1;	
}

void sync_with_leader(char leader[]){
	
	if(LEADER) return;
	char cmd[200];
	bzero(cmd, 200);
	sprintf(cmd, "wget -r --directory-prefix=ftp/synclogs/ ftp://%s/synclogs", leader);
	printf("Getting logs from leader server cmd : %s..\n", cmd);
	system(cmd);
	sprintf(cmd, "mv ftp/synclogs/%s/synclogs/* ftp/synclogs/.", leader);
	system(cmd);
	sprintf(cmd, "rm -r ftp/synclogs/%s", leader);
	system(cmd);
	sprintf(cmd, "wget -r --directory-prefix=ftp/conflicts.file ftp://%s/conflicts.file", leader);
	system(cmd);
	printf("Getting conflicts file from leader server cmd : %s..\n", cmd);
	sprintf(cmd, "mv ftp/conflicts.file/%s/* ftp/conflicts", leader);
	system(cmd);
	sprintf(cmd, "rm -r ftp/conflicts.file");
	system(cmd);
	
	/*for all files in dir, update file in codafs*/
	DIR *dir;
	struct dirent *ent;
	char logsdir[] = "ftp/synclogs/.";
	if ((dir = opendir (logsdir)) != NULL) {
		while ((ent = readdir (dir)) != NULL) {
			if (!(ent->d_type == DT_DIR)){
				int len = strlen(ent->d_name);
				if(ent->d_name[len-4]=='.' && ent->d_name[len-3]=='t' && ent->d_name[len-2]=='x' && ent->d_name[len-1]=='t' )
					continue;
				sync_file(ent->d_name);
			}
			remove(ent->d_name);
		}
	}
	return;
}

void *notify_server(void *arg){
        
        argument *var = (argument *) arg;
	printf("Notifying server for update.%s..\n", var->str2);
        //printf("Argument - %s-file,%s-folder,%s-updater..\n", var->str1, var->str2, var->str3);
        int sockfd, newsockfd, portno;
        
        sockfd = var->sock;
        struct sockaddr_in serv_addr, clnt_addr;

        sockfd = socket(AF_INET, SOCK_STREAM, 0);
        if (sockfd < 0){
                perror("Cannot open socket for connection\n");
                exit(1);
        }
        int i=0;
        bzero((char *) &serv_addr, sizeof(serv_addr));
        serv_addr.sin_family = AF_INET;
        serv_addr.sin_addr.s_addr = inet_addr(var->str2);
        portno = PORT1;
        serv_addr.sin_port = htons(portno);
        
        printf("Trying to connect with server %s..\n", var->str2);
        if(connect(sockfd,(struct sockaddr *)&serv_addr,sizeof(serv_addr))<0){
                printf("Connection not successful with server %s. Cannot notify for syncing.\n\n", var->str2);                
        } else {
                printf("Connection successful with server %s, notifying to sync..\n", var->str2);
		char msg[1024];
                int n = recv(sockfd, msg, 1024, 0);
                sprintf(msg, "leader "SERVER);
                printf("Sending msg to sync servers to %s: %s..\n", var->str2, msg);
                n = send(sockfd, msg, strlen(msg), 0);
                n = recv(sockfd, msg, 1024, 0);
        }
        close(sockfd);
        free(arg);
        pthread_exit(NULL);
}

void 
sync_log_for_dir(const char *logsdir, int server)
{
	DIR *dir;
	struct dirent *ent;
	FILE *fc = fopen("ftp/conflicts.file", "a");
			
	int i;
	int flag=0;
	char filename[100];
	if ((dir = opendir (logsdir)) != NULL) {
	  while ((ent = readdir (dir)) != NULL) {
		if (!(ent->d_type == DT_DIR)){
			flag=0;
			for(i=0;i<num;i++){
				if(i==server) continue;
				sprintf(filename, "logs/servers/%s/%s", servers[i], ent->d_name);
				printf("Checking %s..within %s..\n", filename, logsdir);
				if(access(filename,F_OK)!=-1){
					//printf("Exists..\n");
					fprintf(fc, "%s %s %s\n", ent->d_name, servers[server], servers[i]);
					flag=1;
					//remove(filename);
				}
			}
			sprintf(filename, "ftp/logs/%s", ent->d_name);
			printf("Checked file %s..conflict - %d..\n", ent->d_name, flag);
			if(!flag){
				if(strcmp(servers[server], SERVER)==0) continue;
				char cmd[100];
				sprintf(cmd, "cp logs/servers/%s/%s ftp/synclogs/.", servers[server], ent->d_name);
				//printf("%s\n", cmd);
				system(cmd);
			} 
		}
	  }
	  closedir (dir);
	} else {
	  /* could not open directory */
	  printf("Could not open directory - %s to sync server logs..\n", logsdir);
	}
	fclose(fc);
}

void *
sync_servers(void *arg){
	//get_log_for_dir("ftp/.", 0, 0);
	if(LEADER){
		pthread_t p;
		init_servers();
		//schedule for 12am.
		time_t now, midnight;
		struct tm *tminfo;
		time(&now);
		time(&midnight);
		tminfo = localtime(&midnight);
		tminfo->tm_mday = tminfo->tm_mday+1;
		tminfo->tm_hour = 0;
		tminfo->tm_min = 0;
		tminfo->tm_sec = 1;
		midnight = mktime(tminfo);
		if(midnight==-1) printf("Error in mktime()..\n");
		int seconds;
		seconds = difftime(midnight,now);
		sleep(seconds);		//Wait till midnight to sync.
		
		printf("sleep..\n");
		
		FILE *fc = fopen("ftp/conflicts.file", "a");
		char buff[100];
		time(&now);
    		strftime (buff, 100, "%Y-%m-%d %H:%M:%S.000", localtime(&now));
    		fprintf(fc, "\n***********************%s***********************\n\n", buff);
		fclose(fc);
		
		printf("Sync start..\n");
		int i;
		char serverlogs[100];
		sprintf(serverlogs, LOGDIR "servers");
		for(i=0;i<num;i++){
			//if(strcmp(servers[i],SERVER)==0) continue;
			char cmd[200];
			bzero(cmd, 200);
			sprintf(cmd, "wget -r --directory-prefix=%s/ ftp://%s/logs", serverlogs, servers[i]);
			printf("Getting logs from server cmd : %s..\n", cmd);
			system(cmd);
			sprintf(cmd, "mv logs/servers/%s/logs/* logs/servers/%s/.", servers[i], servers[i]);
			system(cmd);
			sprintf(cmd, "rm -r logs/servers/%s/logs", servers[i]);
			system(cmd);
		}
		for(i=0;i<num;i++){
			//if(strcmp(servers[i],SERVER)==0) continue;
			sprintf(serverlogs, "logs/servers/%s/.", servers[i]);
			sync_log_for_dir(serverlogs, i);			
		}
		/**Notify all servers to sync their files**/
		for(i=0;i<num;i++){
			if(strcmp(servers[i],SERVER)==0) continue;
			argument *arg = (argument *) malloc (sizeof(argument));
                        strcpy(arg->str2, servers[i]);
                        
                        if(pthread_create(&p, NULL, notify_server, arg)){
                                printf("Error in creating a new thread..\n");
                                exit(1);
                        }
		}
		
		sleep(20); //Wait for other servers to download synclogs
		
		/*sync your own*/
		DIR *dir;
		struct dirent *ent;
		char logsdir[] = "ftp/synclogs/.";
		if ((dir = opendir (logsdir)) != NULL) {
			while ((ent = readdir (dir)) != NULL) {
				if (!(ent->d_type == DT_DIR)){
					int len = strlen(ent->d_name);
					if(ent->d_name[len-4]=='.' && ent->d_name[len-3]=='t' && ent->d_name[len-2]=='x' && ent->d_name[len-1]=='t' )
						continue;
					sync_file(ent->d_name);
				}
				remove(ent->d_name);
			}
		}
		
		if(pthread_create(&p, NULL, sync_servers, &newsockfd)){
			printf("Error in creating a new thread..\n");
			exit(1);
		}
	}
	pthread_exit(NULL);
}

void *send_message(void *arg){
		//Connect on port 5002, and send message to client to update file.        
        argument *var = (argument *) arg;
        //printf("Argument - %s-file,%s-folder,%s-updater..\n", var->str1, var->str2, var->str3);
        int sockfd, newsockfd, portno;
        
        sockfd = var->sock;
        struct sockaddr_in serv_addr, clnt_addr;

        sockfd = socket(AF_INET, SOCK_STREAM, 0);
        if (sockfd < 0){
                perror("Cannot open socket for connection\n");
                exit(1);
        }
        int i=0;
        bzero((char *) &serv_addr, sizeof(serv_addr));
        serv_addr.sin_family = AF_INET;
        serv_addr.sin_addr.s_addr = inet_addr(var->str2);
        portno = PORT2;	//5002
        struct timeval tv;
        tv.tv_sec = 2;
        tv.tv_usec = 0;
        
        setsockopt(sockfd, SOL_SOCKET, SO_RCVTIMEO, (char *)&tv, sizeof(struct timeval));
        setsockopt(sockfd, SOL_SOCKET, SO_SNDTIMEO, (char *)&tv, sizeof(struct timeval));
        
        serv_addr.sin_port = htons(portno);
        if(strcmp(var->str3, var->str2)==0){
                printf("Updated file..%s.\n", var->str2);
                //continue; //no need to send notification to same client who updated the file.
        }


        printf("Trying to connect with %s..\n", var->str2);
        if(connect(sockfd,(struct sockaddr *)&serv_addr,sizeof(serv_addr))<0){
                printf("Connection not successful with client %s. Cannot notify.\n\n", var->str2);
                char *fname = (char *) malloc (1024*sizeof(char));
                sprintf(fname, "logs/%s/notifications", var->str2);
                printf("Writing notification file %s for %s..\n", fname, var->str1);
                FILE *notification = fopen(fname, "a");
                if(notification == NULL)
                        printf("File %s Cannot be opened.\n", var->str1);
                else{
                        fprintf(notification, "update %s\n", var->str1);
                }
                fclose(notification);
        } else {
                printf("Connection successful with %s, notifying for update..\n", var->str2);
		char msg[1024];
                int n = recv(sockfd, msg, 1024, 0);
                sprintf(msg, "update %s", var->str1);
                printf("Sending msg to %s: %s..\n", var->str2, msg);
                n = send(sockfd, msg, strlen(msg), 0);
                n = recv(sockfd, msg, 1024, 0);
        }
        close(sockfd);
        free(arg);
        pthread_exit(NULL);
}

int send_messages(const char *name, int level, char *file, char *clnt){
        DIR *dir;
        struct dirent *entry;
        int i=0;
        
        /**
        search for all clients in logs directory, and send them messages to updaate file. 
        **/
        
        if (!(dir = opendir(name)))
                return;
        if (!(entry = readdir(dir)))
                return;

        do {
                if (entry->d_type == DT_DIR) {
                        char path[1024];
                        int len = snprintf(path, sizeof(path)-1, "%s/%s", name, entry->d_name);
                        path[len] = 0;
                        
                        if (strcmp(entry->d_name, ".") == 0 || strcmp(entry->d_name, "..") == 0 || strcmp(entry->d_name, "servers") == 0)
                                continue;
                        //printf("within directory %s, level %d..\n", entry->d_name, level);
                        
                        argument *arg = (argument *) malloc (sizeof(argument));
                        arg->sock = sockfd;
                        strcpy(arg->str1, file);
                        strcpy(arg->str2, entry->d_name);
                        //printf("Argument before calling thread..%s::%s..\n", entry->d_name, arg->str2);
                        strcpy(arg->str3, clnt);
                        
                        if(pthread_create(&p, NULL, send_message, arg)){
                                printf("Error in creating a new thread..\n");
                                exit(1);
                        }
                }
                else {
                        continue;
                        //nothing.
                }
        } while (entry = readdir(dir));
        closedir(dir);

}

void *notify_update(void *filename){
	/**
	File changed at server by clnt, send update messages to other clients, whoever are registered.
	**/
	argument *arg = (argument *) filename;
	char *file = (char *) arg->str1;
	char *clnt = (char *) arg->str2;
	printf("notify update..%s,%s..\n", file, clnt);
	int a = send_messages("logs/.", 0, file, clnt);
	return;
}

void process_command(char cmd[], char **rcvd_strings){
	/**
	This function breaks message from client into three different strings. If cmd[] is "upload a.txt", 
	rcvd_strings[0] will be "upload", rcvd_strings[1] will be "a.txt".
	**/
	int i=0,j=0;
	while(cmd[i]!=' '&&cmd[i]!='\0'&&cmd[i]!='\n'){
		rcvd_strings[0][i] = cmd[i]; i++;
	}
	rcvd_strings[0][i]='\0';
	if(cmd[i]=='\0') return;i++;
	while(cmd[i]!=' '&&cmd[i]!='\0'&&cmd[i]!='\n'){
		rcvd_strings[1][j++] = cmd[i++];
	}
	rcvd_strings[1][j]='\0';j=0;
	if(cmd[i]=='\0') return;i++;
	while(cmd[i]!='\0'&&cmd[i]!='\n'){
		rcvd_strings[2][j++] = cmd[i++];
	}
	rcvd_strings[2][j]='\0';
	return;
}

void *serve_client(void *arg){
	int *a = (int *) arg;
	int sockfd = *a;
	char **rcvd_strings;
	int i;
	printf("serving client..\n");
	rcvd_strings = (char **) malloc (3*sizeof(char*));
	for(i=0;i<3;i++){
		rcvd_strings[i] = (char *) malloc (1024*sizeof(char));
	}
	while(1){
		bzero(buffer,1024);
		int n = recv(sockfd, buffer, 1024, 0);
		if(n<=0){
			printf("Client exited..\n");
			close(sockfd);
			break;
		}
		printf("message received.. %s\n", buffer);
		process_command(buffer,rcvd_strings);
		if(strcmp(rcvd_strings[0], "ping")==0){   //to check connection.
			bzero(buffer,1024);
			sprintf(buffer,"ACK");
			n = send(sockfd, buffer, strlen(buffer), 0);
		} else if(strcmp(rcvd_strings[0], "leader")==0){   //Leader server sent message to sync logs now.
			bzero(buffer,1024);
			sprintf(buffer,"OK");
			n = send(sockfd, buffer, strlen(buffer), 0);
			sync_with_leader(rcvd_strings[1]);
		} else if(strcmp(rcvd_strings[0], "notifications")==0){	//A client got reconnected after disconnection. Wants failed notifications.
			//ftp file
			bzero(buffer,1024);
			sprintf(buffer, "uploading notifications");
			n = send(sockfd, buffer, strlen(buffer), 0);
			bzero(buffer,1024);
			sprintf(buffer, "awk '!seen[$0]++' logs/%s/notifications > logs/%s/notifications.s", rcvd_strings[1], rcvd_strings[1]);
			system(buffer);
			sprintf(buffer,"logs/%s/notifications.s",rcvd_strings[1]);
			FILE *fp = fopen(buffer,"r");
			char cmd[200];
			if(fp!=NULL){	//Read notification file and send all notifications
				while(fgets(cmd, sizeof(cmd),fp)){
					argument *arg = (argument *) malloc (sizeof(argument));
				        arg->sock = sockfd;
				        strcpy(arg->str1, cmd+7);
				        arg->str1[strlen(arg->str1)-1]='\0';
				        strcpy(arg->str2, rcvd_strings[1]);
				        //printf("Argument before calling thread..%s::%s..\n", entry->d_name, arg->str2);
				        strcpy(arg->str3, "not");
				        
				        if(pthread_create(&p, NULL, send_message, arg)){
				                printf("Error in creating a new thread..\n");
				                exit(1);
				        }
				}
			} else{
				printf("Notification file does not exist..\n");
			}

			//send_file(buffer, sockfd);
			int status = remove(buffer);
			if(status==0) printf("Deleted notification.s file for %s..\n", rcvd_strings[1]);
			else printf("Cannot delete notification file for %s..\n", rcvd_strings[1]);
			buffer[strlen(buffer)-2] = '\0';
			status = remove(buffer);
			if(status==0) printf("Deleted notification file for %s..\n", rcvd_strings[1]);
			else printf("Cannot delete notification.s file for %s..\n", rcvd_strings[1]);
			printf("File notifications sent to client %s..\n",rcvd_strings[1]);
		} else if(strcmp(rcvd_strings[0], "register")==0){	//If folder of logs not created for client. Create new. Delete old notifications.
			printf("Registering client - %s..\n", rcvd_strings[1]);
			sprintf(buffer,"logs/%s",rcvd_strings[1]);
			int res = mkdir(buffer, 0777);
			if(res==-1) printf("Client already registered..\n");
			else printf("%s registered..\n",rcvd_strings[1]);
			bzero(buffer,1024);
			sprintf(buffer,"logs/%s/notifications",rcvd_strings[1]);
			int status = remove(buffer);			
			bzero(buffer,1024);
			sprintf(buffer,"1");
			n = send(sockfd, buffer, strlen(buffer), 0);
		} else if(strcmp(rcvd_strings[0], "update")==0){	//File changed at client's system.
			//ftp file
			bzero(buffer,1024);
			sprintf(buffer, "upload %s", rcvd_strings[1]);
			n = send(sockfd, buffer, strlen(buffer), 0);
		} else if(strcmp(rcvd_strings[0], "delete")==0){	//Not used.
			//ftp file
			sprintf(buffer, "logs/%s/notifications.server",rcvd_strings[1]);
			int status = remove(buffer);
			if(status==0) printf("Deleted notification file for %s..\n", rcvd_strings[1]);
			else printf("Cannot delete notification file for %s..\n", rcvd_strings[1]);
			buffer[2]='\0';
			bzero(buffer,1024);
			sprintf(buffer, "OK");
			n = send(sockfd, buffer, strlen(buffer), 0);
		} else if(strcmp(rcvd_strings[0], "uploading")==0){	//Client uploading logs.
			int updateyes = 0;
			sprintf(buffer, "logs/%s/%s", rcvd_strings[2], rcvd_strings[1]);
			receive(buffer, sockfd);
			printf("Received file %s..\n", rcvd_strings[1]);

			argument arg;
			strcpy(arg.str1,rcvd_strings[1]);
			strcpy(arg.str2,rcvd_strings[2]);
			int len = strlen(rcvd_strings[1]);
			if(rcvd_strings[1][len-4]=='.'&&rcvd_strings[1][len-3]=='l'&&rcvd_strings[1][len-2]=='o'&&rcvd_strings[1][len-1]=='g'){
				updateyes = 1;
				update_file(rcvd_strings[1], rcvd_strings[2]);
			}

			if(updateyes){
				if(pthread_create(&p, NULL, notify_update, &arg)){
					printf("Error in creating a new thread..\n");
					exit(1);
				}
			}
			bzero(buffer,1024);
			sprintf(buffer, "File received");
			n = send(sockfd, buffer, strlen(buffer), 0);
		}
		buffer[0]='\0'; 
	}
	printf("Exiting thread..");
	pthread_exit(NULL);
}

int main(int argc, char *argv[])
{
	int *arg;
	sockfd = socket(AF_INET, SOCK_STREAM, 0);
	if (sockfd<0){
		perror("Cannot open socket for connection\n");
	}
	
	//Start thread to sync servers.
	if(pthread_create(&p, NULL, sync_servers, &newsockfd)){
		printf("Error in creating a new thread..\n");
		exit(1);
	}

	bzero((char *) &serv_addr, sizeof(serv_addr));
	portno = PORT1;
	serv_addr.sin_family = AF_INET;
	serv_addr.sin_addr.s_addr = inet_addr(SERVER);
	serv_addr.sin_port = htons(portno);

	if (bind(sockfd, (struct sockaddr *) &serv_addr, sizeof(serv_addr))<0){
		perror("Error - Cannot bind server on given port.\n");
		exit(1);
	}
	
	//Start listening on port 5001, for clients.
	printf("Accepting connections-port %d..\n", portno);
	listen(sockfd,10);
	int clients = sizeof(clnt_addr);
	while(1){
		bzero(buffer,1024);
		printf("Before accepting\n");
		newsockfd = accept(sockfd, (struct sockaddr *) NULL, NULL);
		//Some clients connected.
		printf("After accepting\n");
		if (newsockfd<0)
		{
			perror("Error in accepting connection from clients..\n");
		}
		buffer[0]='\0';
		strcpy(buffer,"ACK");
		printf("Sending msg to client: %s..\n", buffer);
		int n = send(newsockfd, buffer, strlen(buffer), 0);
		//New thread to listen messages of this client..
		if(pthread_create(&p, NULL, serve_client, &newsockfd)){
			printf("Error in creating a new thread..\n");
			exit(1);
		}

	}

	return 1;
}
