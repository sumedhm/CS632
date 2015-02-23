#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <curl/curl.h>
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
#define IP "172.27.30.25"
#define PORT 5001
#define LENGTH 1024
char buffer[1024];
char revbuf[LENGTH];

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
		char ffsname[100];
		strcpy(ffsname,"/home/shujaat/Desktop/CODA/Client/logs/");
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
        sprintf(buffer_recv, "%s.log", file);
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
        printf("BUFFER:: %s\n", buffer_send);
        printf("strlen:: %d\n", strlen(buffer_send));
		n = send(sockfd, buffer_send, strlen(buffer_send), 0);
        bzero(buffer_send,1024);
		n = recv(sockfd, buffer_send, 1024, 0);
		//strcpy(file1, file);
        //printf("File name in send_updates :: %s \n",file);
        bzero(buffer_send,1024);
		sprintf(buffer_send, "uploading %s "IP,file);
        //buffer_send[10+strlen(file)+IP_LEN]='\0';
        //printf("buff - %s\n\n", buffer);
        n = send(sockfd, buffer_send, strlen(buffer_send), 0);
	    printf("File name in send_updates :: %s \n",file);
        send_file(file, sockfd);
        free(buffer_send);
	return 1;
}

int main(){
	int sockfd, newsockfd, portno;
    struct sockaddr_in serv_addr, clnt_addr;

    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0){
        perror("Cannot open socket for connection\n");
        exit(1);
    }
    //int i=0;
    bzero((char *) &serv_addr, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = inet_addr("172.27.30.50");
    portno = PORT;
    serv_addr.sin_port = htons(portno);
    //printf("Checking neighbor %d - %d\n", i, neighbor[i]);

    if(connect(sockfd,(struct sockaddr *)&serv_addr,sizeof(serv_addr))<0){
	printf("Connection not successful..\n");
	return 0;
    } 






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
  wd = inotify_add_watch( fd, "/home/shujaat/Desktop/CODA/Client/logs/", IN_CLOSE_WRITE);

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
                      	int writeyes = 1;
                      	char fname[100];
                      	int len = strlen(event->name);
                      	if(event->name[len-4]=='.'&&event->name[len-3]=='s'&&event->name[len-2]=='w'&&event->name[len-1]=='p')writeyes = 0;
                      	if(event->name[len-5]=='.'&&event->name[len-4]=='s'&&event->name[len-3]=='w'&&event->name[len-2]=='p'&&event->name[len-1]=='x')writeyes = 0;
        				        if(event->name[len-4]=='.'&&event->name[len-3]=='s'&&event->name[len-2]=='w'&&event->name[len-1]=='x')writeyes = 0;
        				        if(event->name[len-4]=='4'&&event->name[len-3]=='9'&&event->name[len-2]=='1'&&event->name[len-1]=='3')writeyes = 0;
        
        				if(writeyes){
                          
                          //sendlog();
        				  printf("Inside writeyes\n");
                          strcpy(fname,event->name);
                          printf( "File Closed after writing :: %s \n", fname );
                          n = send_updates(fname, sockfd);
                          bzero(buffer,1024);
                          n = recv(sockfd, buffer, 1024, 0);

						  printf("msg : %s", buffer);
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