#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h> 
#include <sys/socket.h>
#include <netinet/in.h>
#include <sys/types.h>
#include <unistd.h>
#include <pthread.h>
#define MAX 3
#define ID 3
#define PORT 5003
int neighbor[MAX-1] = {5001,5002};


/* Structure to pass message received to thread */
struct argument{
        int sock;
        char str1[1024];
        char str2[1024];
        char str3[1024];
};
typedef struct argument argument;

int local_id = ID;

char rcvd_strings[3][1000];
int sockfd, newsockfd, portno, neighbor_up, neighbor_index;
char msg[1024];
struct sockaddr_in serv_addr, clnt_addr;
char buffer[1024];
int n;

int NO_LEADER=1;
int ELECTION=0;

void process_command(char cmd[]){
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

int find_neighbor(){
    int sockfd, newsockfd, portno;
    printf("Finding neighbor\n");
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
    portno = neighbor[i];
    serv_addr.sin_port = htons(portno);
    //printf("Checking neighbor %d - %d\n", i, neighbor[i]);
    
    while(connect(sockfd,(struct sockaddr *)&serv_addr,sizeof(serv_addr))<0){
	//neighboring node down. Go to next.
	i = i+1;
	//close(sockfd);
	if(i==MAX-1) return -1;
	printf("Checking neighbor %d - %d\n", i, neighbor[i]);
	portno = neighbor[i];
	serv_addr.sin_port = htons(portno);
    }
    int n = recv(sockfd, buffer, 1024, 0);
    //close(sockfd);
    //save nearest neighbor
    neighbor_up = sockfd;
    return i;
}

void send_to_neighbor(char msg[], int newsockfd, int i){
    printf("Sending msg to neighbor %d - %d\n", i, neighbor[i]);
    int n = send(newsockfd, msg, strlen(msg), 0);
    if(n<0){
	printf("Cannot write to socket. Possibly the node has disconnected..\n");
	exit(1);
    }

}

void *process_msg(void *arg){
	if(!ELECTION){		//If this node has not started election, it has not yet found its neighbor.
		ELECTION=1;
		neighbor_index = find_neighbor();			
	}
	char response[1024];
	response[0] = '\0';
	argument *msg = (argument *) arg;
	int newsockfd = msg->sock;
	//printf("Received - %s %s %s\n", msg->str1, msg->str2, msg->str3);
	if(strcmp(msg->str1,"ELECTION")==0){
		strcpy(response, "ACK");
        	//printf("\nResponse sent-\n%s\n", response);
        	int n = send(newsockfd, response, strlen(response), 0);
        	if(n<0){
                	printf("Cannot write to socket. Possibly the node has disconnected..\n");
        	}

		printf("Processing election message..neighbor - %d:%d.\n\n", neighbor_index, neighbor[neighbor_index]);
		int rcvd_id = atoi(msg->str2);
		if(rcvd_id==ID){
			sprintf(buffer, "LEADER %d", ID);
			send_to_neighbor(buffer, neighbor_up, neighbor_index);
			//printf("Leader message sent..\n");
		} 
		if(rcvd_id <= local_id){
			local_id = rcvd_id;
			sprintf(buffer, "ELECTION %d", rcvd_id);
			send_to_neighbor(buffer, neighbor_up, neighbor_index);
			//printf("Election message sent..\n");
		} else {
			//Forward your own id or better local_id.
			sprintf(buffer, "ELECTION %d", local_id);
                        send_to_neighbor(buffer, neighbor_up, neighbor_index);
		}
        } else if(strcmp(msg->str1,"LEADER")==0){
		strcpy(response, "ACK");
                printf("\nResponse sent-\n%s\n", response);
                int n = send(newsockfd, response, strlen(response), 0);
                if(n<0){
                        printf("Cannot write to socket. Possibly the node has disconnected..\n");
                }
		int rcvd_id = atoi(msg->str2);
		printf("Processing leader message..neighbor - %d:%d, rcvd - %d.\n\n", neighbor_index, neighbor[neighbor_index], rcvd_id);
                if(rcvd_id==ID){
                        //Drop the message. All have been notified.
			printf("\n\nAll have been notified that I am the leader.\n");
			printf("\nMy ID - %d, Leader elected - %d.\n\n", ID, rcvd_id);
			NO_LEADER = 0;
                }
                if(rcvd_id!=local_id){
			//Edit your own local_id(leader) and forward to neighbor.
                        local_id = rcvd_id;
                        sprintf(buffer, "LEADER %d", rcvd_id);
                        send_to_neighbor(buffer, neighbor_up, neighbor_index);
			printf("\nMy ID - %d, Leader elected - %d.\n\n", ID, rcvd_id);
			NO_LEADER = 0;
                } else {
			//Just forward to neighbor.
			sprintf(buffer, "LEADER %d", rcvd_id);
                        send_to_neighbor(buffer, neighbor_up, neighbor_index);
                        printf("\nMy ID - %d, Leader elected - %d.\n\n", ID, rcvd_id);
                        NO_LEADER = 0;
                }
        } else if(strcmp(msg->str1,"ACK")){
		//printf("ack received..\n");
		//ignore
	} else{
		printf("Invalid request, cannot understand command.%s:%s.\n", msg->str1, msg->str2);
	}
	bzero(buffer,1024);
	//printf("msg process exited\n");
	pthread_exit(NULL);
}

void *listen_neighbor(void *arg){
		sockfd = socket(AF_INET, SOCK_STREAM, 0);
    		if (sockfd<0){
        		perror("Cannot open socket for connection\n");
    		}

    		bzero((char *) &serv_addr, sizeof(serv_addr));
    		portno = PORT;
    		serv_addr.sin_family = AF_INET;
	    	serv_addr.sin_addr.s_addr = inet_addr("127.0.0.1");
    		serv_addr.sin_port = htons(portno);

    		if (bind(sockfd, (struct sockaddr *) &serv_addr, sizeof(serv_addr))<0){
         		perror("Error - Cannot bind server on given port.\n");
         		exit(1);
    		}
 		printf("Accepting connections..\n");
		listen(sockfd,10);
		int clients = sizeof(clnt_addr);
	
		bzero(buffer,1024);
		//printf("Before accepting\n");
        	newsockfd = accept(sockfd, (struct sockaddr *) NULL, NULL);
		//printf("After accepting\n");
        	if (newsockfd<0)
        	{
            		perror("Error in accepting connection from clients..\n");
        	}
		strcpy(buffer,"ACK");
		int n = send(newsockfd, buffer, strlen(buffer), 0);
		bzero(buffer,1024);
        	while(1){
			if(!NO_LEADER) break;
			printf("\nWaiting for receive..\n");
			int n = recv(newsockfd, buffer, 1024, 0);
        		if(n<0){
                		printf("Cannot read from socket. Possibly the node has disconnected..\n");
                		close(newsockfd);
        			break;
        		}
			printf("Buffer - %s\n", buffer);
			if(strcmp(buffer,"")==0){
				printf("Client disconnected..\n");
				break;
			}
        		rcvd_strings[0][0]='\0';rcvd_strings[1][0]='\0';rcvd_strings[2][0]='\0';
        		process_command(buffer);	//Decode the message.
        		argument *arg = (argument *) malloc (sizeof(argument));
        		arg->sock = newsockfd;
        		strcpy(arg->str1, rcvd_strings[0]);
        		strcpy(arg->str2, rcvd_strings[1]);
        		strcpy(arg->str3, rcvd_strings[2]);
        		pthread_t th;
        		if(pthread_create(&th, NULL, process_msg, arg)){
                		printf("Error in creating a new thread..\n");
				break;
			}
			bzero(buffer,1024);
			/*int p = fork();
			if(p==0) process_msg(arg);*/
		}
	close(newsockfd);
	printf("neighbor listener exited..\n");
	pthread_exit(NULL);		
}

int main(int argc, char *argv[])
{
	//printf("Creating thread.\n\n");
	pthread_t p;
        int *arg;
        if(pthread_create(&p, NULL, listen_neighbor, arg)){
                printf("Error in creating a new thread..\n");
                exit(1);
        }
	//printf("Thread created..\n");

	local_id = ID;
	
	int i,j;
	printf("Enter 1 to start election.\n");

	//Waiting for nodes to be started.
	scanf("%d", &j);
	i = find_neighbor();
	ELECTION=1;
	neighbor_index = i;
	printf("Current neighbor - %d:%d\n", i, neighbor[i]);
	char msg[1024];
	printf("Sending election msg..\n\n");
        sprintf(msg, "ELECTION %d", ID);
        send_to_neighbor(msg, neighbor_up, i);
        printf("Processing client %d, %d\n", inet_ntoa(clnt_addr.sin_addr.s_addr), (int)clnt_addr.sin_port);
        printf("Msg from client - %s\n", buffer);
	while(NO_LEADER){
		sleep(1);
	}
	exit(1);
}
