#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <curl/curl.h>
#include <netinet/in.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <errno.h>
#include <unistd.h>
#include <pthread.h>
#define MAX 3
#define IP "127.0.0.1"
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
		printf("[Client] Sending %s to the Server... ", fs_name);
		FILE *fs = fopen(fs_name, "r");
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
	buffer[0]='\0';
        sprintf(buffer, "send %s", file);
        n = send(sockfd, buffer, strlen(buffer), 0);
        buffer[0]='\0';
        n = recv(sockfd, buffer, 1024, 0);
	if(n<=0) return 0;
        sprintf(buffer, "%s.log", file);
        receive(buffer, sockfd);
        return 1;
}

int send_updates(char *file, int sockfd){
	int n;
	buffer[0]='\0';
        sprintf(buffer, "update %s", file);
	n = send(sockfd, buffer, strlen(buffer), 0);
        buffer[0]='\0';
	n = recv(sockfd, buffer, 1024, 0);
        sprintf(buffer, "uploading %s " IP, file);
        n = send(sockfd, buffer, strlen(buffer), 0);
	sprintf(buffer, "%s.log", file);
        send_file(buffer, sockfd);
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
    int i=0;
    bzero((char *) &serv_addr, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = inet_addr("127.0.0.1");
    portno = PORT;
    serv_addr.sin_port = htons(portno);
    //printf("Checking neighbor %d - %d\n", i, neighbor[i]);

    if(connect(sockfd,(struct sockaddr *)&serv_addr,sizeof(serv_addr))<0){
	printf("not Connection successful..\n");
	return 0;
    } 
    int n = recv(sockfd, buffer, 1024, 0);
	printf("msg : %s\n", buffer);
	buffer[0]='\0';
	sprintf(buffer, "register " IP);
        n = send(sockfd, buffer, strlen(buffer), 0);
        buffer[0]='\0';
        n = recv(sockfd, buffer, 1024, 0);
	n = send_updates("a.txt", sockfd);
        buffer[0]='\0';
n = recv(sockfd, buffer, 1024, 0);

        printf("msg : %s", buffer);

    close(sockfd);
    //save nearest neighbor*/

//	ftpupload("a.txt","a.txt.tmp,","ftp://127.0.0.1/logs/a.txt","afinal.txt");

    return i;
}
