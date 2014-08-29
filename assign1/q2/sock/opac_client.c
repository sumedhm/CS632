#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h> 
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
int main(int argc, char *argv[])
{
    int sockfd, portno;
    struct sockaddr_in serv_addr;
    struct hostent *server;

    char buffer[1024];

    if(argc<3){
        fprintf(stderr,"Usage: %s hostname port\n", argv[0]);
        exit(0);
    }
    portno = atoi(argv[2]);
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd<0) 
    {
        perror("Error in opening socket for connection..");
        exit(1);
    }
    server = gethostbyname(argv[1]);
    if (server == NULL) {
        fprintf(stderr,"Error - No such host running..\n");
        exit(0);
    }

    bzero((char *) &serv_addr, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    bcopy((char *)server->h_addr, (char *)&serv_addr.sin_addr.s_addr, server->h_length);
    serv_addr.sin_port = htons(portno);

    if(connect(sockfd,(struct sockaddr *)&serv_addr, sizeof(serv_addr))<0) 
    {
        	perror("Error connecting with server, please retry..");
        	exit(1);
    }
    while(1){
    	/*Now read server response*/
	bzero(buffer, 1024);
    	int result = recv(sockfd,buffer,1024,0);
    	if(result<0){
        	perror("Error - cannot read from socket\n");
         	exit(1);
    	}
	printf("%s\n", buffer);
	printf(":~$ ");
	fgets(buffer,1024,stdin);
	if(buffer[0]=='e' && buffer[1]=='x' && buffer[2]=='i' && buffer[3]=='t'){
		int result = send(sockfd,buffer,strlen(buffer),0);
		printf("\nExiting...Bye!\n");
		close(sockfd);
		exit(1);
	}
	//printf("You entered: %s\n", buffer);
	/*Send message to the server*/
	result = send(sockfd,buffer,strlen(buffer),0);
	if (result<0){
		perror("Error - cannot send message to server\n");
		exit(1);
	}
    }
    return 0;
}
