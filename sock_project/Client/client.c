#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
//#include <curl/curl.h>
#include <netinet/in.h>
#include <sys/stat.h>
//#include <fcntl.h>
#include <errno.h>
#include <unistd.h>
#include <pthread.h>
#include <linux/inotify.h>
#include <limits.h>

#define EVENT_SIZE  ( sizeof (struct inotify_event) )
#define EVENT_BUF_LEN     ( 1024 * ( EVENT_SIZE + 16 ) )
#define MAX 3
#define SERVER "172.27.30.196"
#define ftpSERVER "172.27.30.196/codafs"
#define IP "172.27.30.194"
#define myfsD "/home/optimus/ds/sock_project/Client/example/"
#define myfslogD "/home/optimus/ds/sock_project/Client/logs/"
#define fldUp "/home/optimus/ds/sock_project/Client/fldUp.txt"
#define fldUpC "/home/optimus/ds/sock_project/Client/fldUp.cpy"
#define Conflict "/home/optimus/ds/sock_project/Client/conflicts.txt"
#define ConflictD "/home/optimus/ds/sock_project/Client/cons/"
#define PORT1 5001
#define PORT2 5002
#define LENGTH 1024
char buffer[1024];
char revbuf[LENGTH];
int sockfd, newsockfd, portno;
struct sockaddr_in serv_addr, clnt_addr;
int checkfldUp=0;
int ifconnected; 
int checkingConn = 0;   
int tnotify = 1;
void* updatefile(char* fname,char* fnamel);
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


void process_command(char cmd[], char **rcvd_strings){
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


int send_file(char *fs_name, int sockfd){
	char sdbuf[LENGTH]; 
	char ffsname[100];
	char ffsnamel[100];
	strcpy(ffsname,myfslogD);
	strcat(ffsname,fs_name);
	printf("[Client] Sending %s to the Server... ", ffsname);
	FILE *fs = fopen(ffsname, "r");
	if(fs == NULL)
	{
		printf("ERROR: File %s not found.\n", fs_name);
		exit(1);
	}

	bzero(sdbuf, LENGTH); 
	int fs_block_sz; 
	while((fs_block_sz = fread(sdbuf, sizeof(char), LENGTH, fs)) > 0)
	{
		if(sendall(sockfd, sdbuf, &fs_block_sz) < 0)
		{
			fprintf(stderr, "ERROR: Failed to send file %s. (errno = %d)\n", fs_name, errno);
			return 0;
		}
		bzero(sdbuf, LENGTH);
	}
	return 1;
}

int receive(char *fr_name, int sockfd){
	FILE *fr = fopen(fr_name, "a");
	if(fr == NULL)
		printf("File %s Cannot be opened.\n", fr_name);
	else
	{
		bzero(revbuf, LENGTH); 
		int fr_block_sz = 0;
		while((fr_block_sz = recv(sockfd, revbuf, LENGTH, 0)) > 0)
		{
			int write_sz = fwrite(revbuf, sizeof(char), fr_block_sz, fr);
			if(write_sz < fr_block_sz)
			{
				error("File write failed.\n");
			}
			bzero(revbuf, LENGTH);
			if (fr_block_sz == 0 || fr_block_sz != 512) 
			{
				break;
			}
		}
		if(fr_block_sz < 0)
		{
			if (errno == EAGAIN)
			{
				printf("recv() timed out.\n");
			}
			else
			{
				fprintf(stderr, "recv() failed due to errno = %d\n", errno);
			}
		}
		printf("Ok received from server!\n");
		fclose(fr);
	}
}

int receive_updates(char *file, int sockfd){
	int n;
	char buffer_recv[1024];
	buffer_recv[0]='\0';
	bzero(buffer_recv,1024);
	sprintf(buffer_recv, "send %s", file);
	n = send(sockfd, buffer_recv, strlen(buffer_recv), 0);
	buffer_recv[0]='\0';
	bzero(buffer_recv,1024);
	n = recv(sockfd, buffer_recv, 1024, 0);
	if(n<=0) return 0;
	bzero(buffer_recv,1024);
	sprintf(buffer_recv, "%s", file);
	receive(buffer_recv, sockfd);
	return 1;
}

int send_updates(char *file, int sockfd){
	//char ip[]="172.27.30.25";
	//char *file1 = (char *) malloc (100*sizeof(char));
	//strcpy(file1, file);
	char *buffer_send = malloc(sizeof(char)*1024);
	int n;
	bzero(buffer_send,1024);
	printf("File name in send_updates :: %s \n",file);
	sprintf(buffer_send, "update %s", file);
	//buffer_send[7+strlen(file)]='\0';
	//printf("BUFFER:: %s\n", buffer_send);
	//printf("strlen:: %d\n", strlen(buffer_send));
	n = send(sockfd, buffer_send, strlen(buffer_send), 0);
	if(n<=0) return -1;
  bzero(buffer_send,1024);
  printf("File name in send_updates before recv :: %s \n",file);
	n = recv(sockfd, buffer_send, 1024, 0);
  if(n<=0) return -1;
  printf("File name in send_updates after recv :: %s \n",file);
	//strcpy(file1, file);
	//printf("File name in send_updates :: %s \n",file);
	bzero(buffer_send,1024);
	sprintf(buffer_send, "uploading %s "IP,file);
	//buffer_send[10+strlen(file)+IP_LEN]='\0';
	//printf("buff - %s\n\n", buffer);
	n = send(sockfd, buffer_send, strlen(buffer_send), 0);
  if(n<=0) return -1;
	printf("File name in send_updates :: %s \n",file);
	n = send_file(file, sockfd);
	free(buffer_send);
	if(n<=0)return -1;
	return 1;
}

void *check_connection(void *arg){
    printf("Checking connection - %d..\n", ifconnected);
    char buffer[1024];
    
    //bzero(buffer,1024);
    if(checkingConn==0){
    	checkingConn=1;
    }else{
    	printf("check_connection ALREADY RUNNING. EXITING THREAD\n");
    	pthread_exit(NULL);
    }
    checkfldUp = 1;
    
    int n = -1;
    //printf("2Checking connection - %d..\n", ifconnected);
    while(!ifconnected){
    	bzero(buffer,1024);
    	sprintf(buffer, "ping");
	    n = send(sockfd, buffer, strlen(buffer), 0);
	    
	    if(n<=0){
	    	printf("Ping failed . Disconnected\n");
	    	ifconnected = 0;
	    	sleep(2);
	    }
	    else{
	    	bzero(buffer,1024);
		    printf("Ping done succesfully :: ret value %d\n", n);
		    n = recv(sockfd, buffer, 1024, 0);
		    	if(n<=0){
		    		printf("Ack not received. Disconnected\n");
		    		ifconnected=0;
		    	}
		    	else{
		    		printf("Ack recv succesfully :: ret value %d and \nbuffer :: %s\n",n,buffer);
		    		ifconnected = 1;
	    		}
	    }
    
      sleep(2);
    
    } 
    //ifconnected = 1;
    char cmd[]="awk '!seen[$0]++' fldUp.txt > fldUp.cpy";
    printf("Exiting check connection with ifconnected val :: %d\n", ifconnected);
    
    system(cmd);
    bzero(buffer,1024);
    sprintf(buffer, "notifications "IP);
    n = send(sockfd, buffer, strlen(buffer), 0);

    bzero(buffer,1024);
    n = recv(sockfd, buffer, 1024, 0);

    if(n<=0){
		printf("Ack not received even after reconnection. Disconnected\n");
		//ifconnected=0;
	}
	else{
		printf("Ack recv succesfully :: ret value %d and buffer :: %s\n",n,buffer);
		sleep(5);
		
		char tname[100];
		FILE* fff = fopen(fldUpC,"r");
		checkfldUp=0;
		while(fscanf(fff, "%s",tname)==1){
			char fname[100],fnamel[100];
		//
			strcpy(fname,tname);
			int slen = strlen(fname);
			int lopp;
			for(lopp=0;lopp<slen-4;lopp++){
				fnamel[lopp]=fname[lopp];
			}
			fnamel[slen-4]='.';fnamel[slen-3]='t';fnamel[slen-2]='x';
			fnamel[slen-1]='t';fnamel[slen]='\0';
			printf("tname:: %s\n",tname);
			printf("fldUp filenames fname:: %s &&fnamel:: %s\n",fname,fnamel);
			updatefile(fname,fnamel);

		}
		fclose(fff);
		char cmd2[]="rm -r fldUp.txt fldUp.cpy";
		system(cmd2);
	}

        
}


void *start_server(void *arg){
	int *gsockfd = (int *) arg;
	int sockfd,newsockfd;
	struct sockaddr_in serv_addr, clnt_addr;
	sockfd = socket(AF_INET, SOCK_STREAM, 0);
	if (sockfd<0){
		perror("Cannot open socket for connection\n");
	}

	bzero((char *) &serv_addr, sizeof(serv_addr));
	int portno = PORT2;
	serv_addr.sin_family = AF_INET;
	serv_addr.sin_addr.s_addr = inet_addr(IP);
	serv_addr.sin_port = htons(portno);
	char **rcvd_strings;
	int i;
	printf("serving client..\n");
	rcvd_strings = (char **) malloc (3*sizeof(char*));
	for(i=0;i<3;i++){
		rcvd_strings[i] = (char *) malloc (1024*sizeof(char));
	}
	if (bind(sockfd, (struct sockaddr *) &serv_addr,
				sizeof(serv_addr))<0){
		perror("Error - Cannot bind server on given port.\n");
		exit(1);
	}
	printf("Accepting connections-port %d..\n", portno);
	listen(sockfd,10);
	int clients = sizeof(clnt_addr);
	while(1){
		bzero(buffer,1024);
		printf("Before accepting\n");
		newsockfd = accept(sockfd, (struct sockaddr *) NULL, NULL);
		printf("After accepting\n");
		if (newsockfd<0)
		{
			perror("Error in accepting connection from clients..\n");
		}

		
    else{
      buffer[0]='\0';
  		strcpy(buffer,"ACK");
  		printf("Sending msg to client: %s..\n", buffer);
  		int n = send(newsockfd, buffer, strlen(buffer), 0);
  		while(1){
  			bzero(buffer,sizeof(buffer));
  			n = recv(newsockfd, buffer, 1024, 0);
  			if(n<=0){
  				printf("Disconnected from server..\n");
  				close(newsockfd);
  				break;
  			}
  			printf("message received.. %s\n", buffer);
  			process_command(buffer,rcvd_strings);
  			if(strcmp(rcvd_strings[0], "update")==0){   //to check connection.
  				//receive_updates(rcvd_strings[1],*gsockfd);
          sprintf(buffer,"ACK");
          n = send(newsockfd, buffer, strlen(buffer), 0);
          char ffname[100];
          char fname[100];
          char fnamel[100];
          int len = strlen(rcvd_strings[1]);
          int loop=0;
          for(loop=0;loop<len-4;loop++){
              if(rcvd_strings[1][loop]=='@'){
              	fname[loop]='/';
              	fnamel[loop]='@';
              }
              else{
				fname[loop] = rcvd_strings[1][loop];
				fnamel[loop] = rcvd_strings[1][loop];
			  }




          }
          fname[loop]='\0';
          fnamel[loop]='\0';
          strcpy(ffname,myfsD);
          strcat(ffname,fname);


          printf("File to be updated as told by server:: %s\n and tnotify == %d\n",ffname,tnotify);

          tnotify = 0;
          char cmd1[200];


          if(checkfldUp){
	          	int conflict = 0;
	          	FILE *fp;
	  			char path[1024];
	  			char cmd3[200];
	  			sprintf(cmd3,"grep %s fldUp.cpy",fname);

	  			/* Open the command for reading. */
	  			fp = popen(cmd3, "r");
	  			if (fp == NULL) {
	    			printf("Failed to run command\n" );
	    			exit(1);
	  			}

				/* Read the output a line at a time - output it. */
	  			while (fgets(path, sizeof(path)-1, fp) != NULL) {
	    			conflict = 1;

	    
	  			}
	  			pclose(fp);
	  			if(conflict){
	  				FILE* conflict = fopen(Conflict,"a");
	  				fprintf(conflict, "%s\n",fname);
	  				fclose(conflict);
	  				char llname[100];
	  				char lname[100];
	  				char cllname[100];
	  				char clname[100];
	  				char name[100];
	  				strcpy(llname,myfslogD);
	  				strcpy(cllname,ConflictD);
	  				strcat(llname,fnamel);
	  				strcat(cllname,fnamel);
	  				strcat(llname,".log");
	  				strcat(cllname,".log");
	  				strcat(lname,myfslogD);
	  				strcat(clname,ConflictD);
	  				strcat(lname,fnamel);
	  				strcat(clname,fnamel);
	  				strcat(lname,".txt");
	  				strcat(clname,".txt");
	  				char cmd4[200],cmd5[200];
	  				
	  				sprintf(cmd4,"cat %s >> %s",llname,cllname);
	  				sprintf(cmd5,"cat %s >> %s",lname,clname);
	  				printf("cmd4 :: %s\n",cmd4);
	  				printf("cmd4 :: %s\n",cmd5);
	  				system(cmd4);
	  				system(cmd5);

  			}

          	//Conflict
          	sprintf(cmd1,"sed -i '/%s/d' fldUp.cpy",fname); 
          	system(cmd1);
          }
          char cmd[200];
              strcpy(cmd,"wget ftp://");
              strcat(cmd,ftpSERVER);
              strcat(cmd,"/");
              strcat(cmd,fname);
              strcat(cmd," -O ");
              strcat(cmd,ffname);
              printf("Final cmd:: %s\n",cmd);
              system(cmd);

          tnotify = 1;
  				
        }
  		}
		}
	}
}

	void* updatefile(char* fname,char* fnamel){
		pthread_t p;
						char buffer[1024];
					printf("Inside iflog\n");
						int n2=-1;
						int n1=-1;
						
						printf( "File Closed after writing :: %s \n", fname );
						n1 = send_updates(fnamel, sockfd);
						//n2 = send_updates(fnamel, sockfd);
						printf("value of n1 after send_updates1 :: %d\n",n1);
          				if(n1==-1) {
		                	ifconnected = 0;
		                	printf("Send update failed\n");
		                	if(pthread_create(&p, NULL, check_connection,&sockfd)){
		                      printf("Cannot create a new thread to check_connection..\n");
		                      exit(1);
            				}
          				}
          				else{
							bzero(buffer,1024);
							n1 = recv(sockfd, buffer, 1024, 0);
							printf("After recv in inotify loop value of n1: %d\n",n1);
          					if(n1==-1) {
            				ifconnected = 0;
            					if(pthread_create(&p, NULL, check_connection,&sockfd)){
                  					printf("Cannot create a new thread to check_connection..\n");
                  					exit(1);
            					}
          					}else{

          						printf("msg recv on sending 1st file : %s\n", buffer);
          					}
						}
						if(ifconnected){
							n2 = send_updates(fname, sockfd);
							//n2 = send_updates(fnamel, sockfd);
							printf("value of n2 after send_updates :: %d\n",n2);
              				if(n2==-1) {
			                	ifconnected = 0;
			                	printf("Send update failed\n");
			                	if(pthread_create(&p, NULL, check_connection,&sockfd)){
			                      printf("Cannot create a new thread to check_connection..\n");
			                      exit(1);
                				}
              				}
              				else{
								bzero(buffer,1024);
								n2 = recv(sockfd, buffer, 1024, 0);
								printf("After recv in inotify loop value of n2: %d\n",n2);
              					if(n2==-1) {
                				ifconnected = 0;
                					if(pthread_create(&p, NULL, check_connection,&sockfd)){
                      					printf("Cannot create a new thread to check_connection..\n");
                      					exit(1);
                					}
              					}else{

              						printf("msg recv on sending 2nd file: %s\n", buffer);
              					}
							}
						}
					if((n1!=-1)&&(n2!=-1)){
						printf("Update successful :: %s\n",fname);
					}
					else{
						
						printf("Update failed during uploading :: %s\n",fname);
						FILE* fff = fopen(fldUp,"a");
						fprintf(fff, "%s\n",fname);
						fclose(fff);

					}

	}

	int main(){

		sockfd = socket(AF_INET, SOCK_STREAM, 0);
		if (sockfd < 0){
			perror("Cannot open socket for connection\n");
			exit(1);
		}
		//int i=0;
		struct timeval tv;

		tv.tv_sec = 2;  /* 30 Secs Timeout */
		tv.tv_usec = 0;  // Not init'ing this can cause strange errors

		setsockopt(sockfd, SOL_SOCKET, SO_RCVTIMEO, (char *)&tv,sizeof(struct timeval));
		setsockopt(sockfd, SOL_SOCKET, SO_SNDTIMEO, (char *)&tv,sizeof(struct timeval));
		
		bzero((char *) &serv_addr, sizeof(serv_addr));
		serv_addr.sin_family = AF_INET;
		serv_addr.sin_addr.s_addr = inet_addr(SERVER);
		portno = PORT1;
		serv_addr.sin_port = htons(portno);
		//printf("Checking neighbor %d - %d\n", i, neighbor[i]);

		if(connect(sockfd,(struct sockaddr *)&serv_addr,sizeof(serv_addr))<0){
			printf("Connection not successful..\n");
      		return 0;
		} 
    	ifconnected = 1;
		bzero(buffer,1024);
		int n = recv(sockfd, buffer, 1024, 0);
		printf("msg : %s\n", buffer);
		bzero(buffer,1024);
		sprintf(buffer, "register " IP);
		n = send(sockfd, buffer, strlen(buffer), 0);
		buffer[0]='\0';
		bzero(buffer,1024);
		n = recv(sockfd, buffer, 1024, 0);
		buffer[0]='\0';
		pthread_t p;
		if(pthread_create(&p, NULL, start_server,&sockfd)){
			printf("Cannot create a new thread to start server..\n");
			exit(1);
		}
		


		int length, i = 0;
		int fd;
		int wd;
		char inotifybuffer[EVENT_BUF_LEN];

		/*creating the INOTIFY instance*/
		fd = inotify_init();

		/*checking for error*/
		if ( fd < 0 ) {
			perror( "inotify_init" );
		}

		/*addin
		  g the “/tmp” directory into watch list. Here, the suggestion is to validate the existence of the directory before adding into monitoring list.*/
		wd = inotify_add_watch( fd, myfslogD, IN_CLOSE_WRITE);

		/*read to determine the event change happens on “/tmp” directory. Actually this read blocks until the change event occurs*/ 
		while(1){
			i=0;
			length = read( fd, buffer, EVENT_BUF_LEN );

			/*checking for error*/
			if ( length < 0 ) {
				perror( "read" );
			}


			/*actually read return the list of change events happens. Here, read the change event one by one and process it accordingly.*/
			while ( i < length ) {     
				struct inotify_event *event = ( struct inotify_event * ) &buffer[ i ];
				if ( event->len ) {
					if ( event->mask & IN_CLOSE_WRITE ) {
						//int writeyes = 1;
						int n1=-1,n2=-1;
						int iflog = 0;
						char fname[100];
						char fnamel[100];
						strcpy(fname,event->name);
						int slen = strlen(fname);
						printf("Inotify alerted with filename :: %s\n",fname);
						//int len = strlen(event->name);
						if(fname[slen-4]=='.'&&fname[slen-3]=='l'&&fname[slen-2]=='o'&&fname[slen-1]=='g'){
							iflog=1;
						}						
						
						//if(event->name[len-5]=='.'&&event->name[len-4]=='s'&&event->name[len-3]=='w'&&event->name[len-2]=='p'&&event->name[len-1]=='x')writeyes = 0;
						//if(event->name[len-4]=='.'&&event->name[len-3]=='s'&&event->name[len-2]=='w'&&event->name[len-1]=='x')writeyes = 0;
						//if(event->name[len-4]=='4'&&event->name[len-3]=='9'&&event->name[len-2]=='1'&&event->name[len-1]=='3')writeyes = 0;

						if(iflog&&tnotify){
							int lopp;
							for(lopp=0;lopp<slen-4;lopp++){
								fnamel[lopp]=fname[lopp];


							}
							fnamel[slen-4]='.';fnamel[slen-3]='t';fnamel[slen-2]='x';
							fnamel[slen-1]='t';fnamel[slen]='\0';

							printf("Two files to be uploaded %s :: %s\n",fname,fnamel);

							if(ifconnected==0){
								printf("Update failed because already disconn :: %s\n",fname);
								FILE* fff = fopen(fldUp,"a");
								fprintf(fff, "%s\n",fname);
								fclose(fff);
								//write to failedupdate()
							}
							else{
								updatefile(fname,fnamel);
														
							}
						}

					}
				}
				i += EVENT_SIZE + event->len;
			}
		}
		/*removing the “/tmp” directory from the watch list.*/
		inotify_rm_watch( fd, wd );

		/*closing the INOTIFY instance*/
		close( fd );







		close(sockfd);
		//save nearest neighbor*/

		//	ftpupload("a.txt","a.txt.tmp,","ftp://127.0.0.1/logs/a.txt","afinal.txt");

		return 0;
	}
