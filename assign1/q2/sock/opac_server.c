#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h> 
#include <sys/socket.h>
#include <netinet/in.h>
#define MAX 1000

struct issue_list{
	int copy_id;
        char name[100];
        char issue_date[14];
        char due_date[14];
        struct issue_list * next;
};
typedef struct issue_list issue_list;

typedef struct res_list{
	char name[100];
	char date[14];
	struct res_list *next;
} res_list;

typedef struct _book{
	int book_id;
	char name[256];
	char author[256];
	int copies;
	int left;
	issue_list *issued_to;
	issue_list *list_tail;
	res_list *reserved;
	res_list *r_tail;
} books;

typedef struct Booklist{
	books *book;
	struct Booklist *next;
} booklist;

//typedef struct Book_list book_list;

books *temp = NULL;;
booklist *head = NULL, *tail=NULL, *list=NULL;

int matches = 0;
int num_books = 0;
int issued = 1;
char rcvd_strings[3][1000];
char response[1000];
char buffer[1024];
int sockfd, newsockfd, portno;


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

void welcome();

int show_all(){
	list = head;
        char cat[1000]={'\n'};
	char c[200];
        while(list!=NULL){
        	sprintf(c, "%2d | %20s | %20s | %d/%d\n", list->book->book_id, list->book->name, list->book->author, list->book->left, list->book->copies);
		strcat(cat,c);
        	list = list->next;
	}
	printf("----------------------------------------\n%s\n------------------------------------------------------\n", cat);
	return 0;
}

int match(char text[], char pattern[]){
	/*Simple pattern matching program*/
	int c,d,e,text_length,pattern_length, position=-1;
	text_length=strlen(text);
	pattern_length=strlen(pattern);
	if(pattern_length>text_length) return -1;
	for(c=0;c<=text_length-pattern_length;c++){
		position=e=c;
		for(d=0;d<pattern_length;d++){
			if(tolower(pattern[d])==tolower(text[e])) e++;
			else break;
		}
		if(d==pattern_length) return position;
	}
	return -1;
}

int search(char *string, int search_type){
	matches = 0;
	printf("Searching %d %s\n\n", search_type,string);
	list = head;
	char cat[1000];
	strcpy(response, "------------------------------\n");
	while(list!=NULL){
		if(search_type==1){
			if((strcmp(list->book->name, string)==0) || (match(list->book->name, string))>-1 || (match(list->book->author, string))>-1){
				matches++;
				sprintf(cat, "%2d | %20s | %20s | %d/%d\n", list->book->book_id, list->book->name, list->book->author, list->book->left, list->book->copies);
				strcat(response, cat);
			}
		} else if(search_type==2){
			if(list->book->book_id == atoi(string)){
				matches++;
				temp = list->book;
				sprintf(cat, "%2d | %20s | %20s | %d/%d\n", list->book->book_id, list->book->name, list->book->author, list->book->left, list->book->copies);
                                strcat(response, cat);
				return 0;
                        }
		} else {
			opac_error("Could not understand request, search failed..\n");
			return 0;
		}
		list = list->next;
	}
	strcat(response, "--------------------------------\n\0");
	sprintf(cat, "%d matches found..\n", matches);
	strcat(cat, response);
	strcpy(response, cat);
	return 0;
}

int add(char *bookname, int add_type){
	printf("Adding %d %s\n\n",add_type,bookname);
	strcpy(response, "Adding book\n");
	char cat[1000];
	if(add_type==1){
		search(bookname, 1);
		if(matches){
			strcpy(cat, "Your book title has following matches. You can add a copy to existing book using 'add copy book_id'. Are you sure you want to add a new book?(y/n)\n");
			strcat(cat, response);
			strcpy(response, cat);
		} else {
			strcpy(cat, "Are you sure you want to add a new book with name '");
			strcat(cat, bookname);
			strcat(cat, "'?(y/n)\n");
			strcpy(response, cat);
		}
		bzero(buffer,1024);
		send(newsockfd, response, strlen(response), 0);
		int n = recv(newsockfd, buffer, 1024, 0);
		if(buffer[0]=='y'){
			if(head==NULL){
				head = (booklist *)malloc(sizeof(booklist));
				head->book = (books *)malloc(sizeof(books));
				temp = head->book;
				tail = head;
				head->next = NULL;
			} else {
				tail->next = (booklist *)malloc(sizeof(booklist));
				tail = tail->next;
				tail->next = NULL;
				tail->book  = (books *)malloc(sizeof(books));
				temp = tail->book;
			}
			temp->book_id=++num_books;
			temp->copies=temp->left=1;
			temp->issued_to = NULL;	temp->list_tail = NULL;temp->reserved=NULL;temp->r_tail=NULL;
			strcpy(temp->name, bookname);
			strcpy(response, "Please enter author name(press enter to leave blank)\n");
			bzero(buffer, 1024);
			send(newsockfd, response, strlen(response), 0);
		        int n = recv(newsockfd, buffer, 1024, 0);
			if(buffer[0]=='\n') temp->author[0] = '\0';
			else{
				buffer[strlen(buffer)-1] = '\0';
				strcpy(temp->author, buffer);
			}
			strcpy(response, "Book added successfully.\nBook title - ");
			strcat(response, bookname);
			strcat(response, "\nBook id - ");
			sprintf(cat, "%d", temp->book_id);
                        strcat(response, cat);
			strcat(response, "\nAuthor - ");
			strcat(response, temp->author);
		} else {
			welcome();
		}
	} else if(add_type==2){
		search(bookname, 2);
		if(matches){
			strcpy(cat, "Are you sure you want to add a copy of this book?(y/n)\n");
			strcat(cat, response);
			strcpy(response, cat);
			bzero(buffer,1024);
	                send(newsockfd, response, strlen(response), 0);
        	        int n = recv(newsockfd, buffer, 1024, 0);
                	if(buffer[0]=='y'){
				temp->copies = temp->copies + 1;
				temp->left = temp->left + 1;
				strcpy(response, "Book added successfully.\nBook title - ");
                                strcat(response, bookname);
                                strcat(response, "\nBook id - ");
                                sprintf(cat, "%d", temp->book_id);
				strcat(response, cat);
                                strcat(response, "\nAuthor - ");
                                strcat(response, temp->author);
			} else welcome();
		} else {
			opac_error("Invalid book_id, no such book exists.. Adding copy failed..\n");
			return 0;
		}
	} else {
		opac_error("Could not understand request, adding book failed..\n");
                return 0;
	}
	return 0;
}

int reserve(int book_id){
	printf("Reserving %d\n\n", book_id);
        char cat[1000];
        sprintf(cat, "%d", book_id);
        int res = search(cat, 2);
        if(matches){
        	strcpy(response, "Please enter your name(0 to cancel)\n");
                bzero(buffer,1024);
                send(newsockfd, response, strlen(response), 0);
                int n = recv(newsockfd, buffer, 1024, 0);
                if(buffer[0]!='0'){
                	temp->left = temp->left - 1;
                        if(buffer[strlen(buffer)-1]=='\n') buffer[strlen(buffer)-1]='\0';
                        if(temp->reserved == NULL){
                        	temp->reserved = (res_list *)malloc(sizeof(res_list));
                        	temp->r_tail = temp->reserved;
                        } else {
                                temp->r_tail->next = (res_list *)malloc(sizeof(res_list));
                                temp->r_tail = temp->r_tail->next;
                        }
                        res_list *temp1 = temp->r_tail;
                        temp1->next = NULL;
                        strcpy(temp1->name, buffer);
                        strcpy(response, "Please enter today's date\n");
                        bzero(buffer,1024);
                        send(newsockfd, response, strlen(response), 0);
                        int n = recv(newsockfd, buffer, 1024, 0);
                        if(buffer[strlen(buffer)-1]=='\n') buffer[strlen(buffer)-1]='\0';
                        strcpy(temp1->date, buffer);
			sprintf(response, "Book reservation of book '%s' made on '%s' for '%s'..\n", temp->name, temp1->date, temp1->name);
		} else strcpy(response, "Operation cancelled..\n");
        } else {
                opac_error("Invalid book_id, no such book exist.. Operation failed..\n");
                return 0;
        }
	return 0;
}

int issue(int book_id){
	printf("Issuing %d\n\n", book_id);
	char cat[1000];
	sprintf(cat, "%d", book_id);
        int res = search(cat, 2);
	if(matches){
		if(temp->left>0){
			strcpy(response, "Please enter your name(0 to cancel)\n");
                        bzero(buffer,1024);
                        send(newsockfd, response, strlen(response), 0);
                        int n = recv(newsockfd, buffer, 1024, 0);
                        if(buffer[0]!='0'){
                                temp->left = temp->left - 1;
                                if(buffer[strlen(buffer)-1]=='\n') buffer[strlen(buffer)-1]='\0';
				if(temp->issued_to == NULL){
					temp->issued_to = (issue_list *)malloc(sizeof(issue_list));
					temp->list_tail = temp->issued_to;
				} else {
					temp->list_tail->next = (issue_list *)malloc(sizeof(issue_list));
					temp->list_tail = temp->list_tail->next;
				}
				issue_list *temp1 = temp->list_tail;
				temp1->next = NULL;
				temp1->copy_id = (issued++)%MAX;
				strcpy(temp1->name, buffer);
				strcpy(response, "Please enter issue date\n");
	                        bzero(buffer,1024);
        	                send(newsockfd, response, strlen(response), 0);
                	        int n = recv(newsockfd, buffer, 1024, 0);
				if(buffer[strlen(buffer)-1]=='\n') buffer[strlen(buffer)-1]='\0';
				strcpy(temp1->issue_date, buffer);
				strcpy(response, "Please enter due date\n");
                                bzero(buffer,1024);
                                send(newsockfd, response, strlen(response), 0);
                                n = recv(newsockfd, buffer, 1024, 0);
                                if(buffer[strlen(buffer)-1]=='\n') buffer[strlen(buffer)-1]='\0';
                                strcpy(temp1->due_date, buffer);
				sprintf(response, "Book '%s' successfully issued to '%s' from '%s' to '%s' with copy_id '%d'..\n", temp->name, temp1->name, temp1->issue_date, temp1->due_date, temp1->copy_id);
                        } else strcpy(response, "Issuing cancelled!\n");

		} else {
			opac_error("No copy of requested book is available..You may reserve book using - \"book book_id\"..\n");
                	return 0;
		}
	} else {
		opac_error("Invalid book_id, no such book exist.. Cannot issue..\n");
                return 0;
	}
	return 0;
}

int return_book(int book_id, int copy_id){
	printf("Returning %d %d\n\n", book_id, copy_id);
        char cat[1000];
        sprintf(cat, "%d", book_id);
        int res = search(cat, 2);
        if(matches){
			issue_list *temp1 = temp->issued_to, *temp2=temp->issued_to;
			int flag=1;
			while(temp1!=NULL){
				if(temp1->copy_id==copy_id){
					flag=0;break;
				}
				temp2 = temp1;
				temp1 = temp1->next;
			}
			if(flag){
				strcpy(response, "Operation failed.. Invalid copy_id..\n");return 0;
			}
            temp->left = temp->left+1;
            if(temp1==temp->issued_to) temp->issued_to=NULL;
			temp2->next = temp1->next;
			free(temp1);
			strcpy(response, "Book has been returned successfully..\n");
        } else {
                opac_error("Invalid book_id, no such book exist.. Operation failed..\n");
                return 0;
        }
	return 0;
}


int renew(int book_id, int copy_id){
	printf("Renewing %d %d\n\n", book_id, copy_id);
        char cat[1000];
        sprintf(cat, "%d", book_id);
        int res = search(cat, 2);
        if(matches){
			issue_list *temp1 = temp->issued_to, *temp2=temp->issued_to;
			int flag=1;
			while(temp1!=NULL){
				if(temp1->copy_id==copy_id){
					flag=0;break;
				}
				temp1 = temp1->next;
			}
			if(flag){
				strcpy(response, "Operation failed.. Invalid copy_id..\n");return 0;
			}
			strcpy(response, "Please enter issue date\n");
	                        bzero(buffer,1024);
        	                send(newsockfd, response, strlen(response), 0);
                	        int n = recv(newsockfd, buffer, 1024, 0);
				if(buffer[strlen(buffer)-1]=='\n') buffer[strlen(buffer)-1]='\0';
				strcpy(temp1->issue_date, buffer);
				strcpy(response, "Please enter due date\n");
                                bzero(buffer,1024);
                                send(newsockfd, response, strlen(response), 0);
                                n = recv(newsockfd, buffer, 1024, 0);
                                if(buffer[strlen(buffer)-1]=='\n') buffer[strlen(buffer)-1]='\0';
                                strcpy(temp1->due_date, buffer);
            sprintf(response, "Book '%s' has been renewed for '%s' from '%s' to '%s' successfully..\n", temp->name, temp1->name, temp1->issue_date, temp1->due_date);
        } else {
                opac_error("Invalid book_id, no such book exist.. Operation failed..\n");
                return 0;
        }
	return 0;
}


int opac_error(char err[1000]){
	strcpy(response, err);
	return 0;
}

void welcome(){
	strcpy(response,"---------------------------------\nPlease enter command:\nTo search:\n\tTo search by name or author - search name 'search_string'\n\tTo search by id - search id 'book_id'\nTo add a book:\n\tTo add new - add new book_name\n\tTo add copy of existing book - add copy book_id\nTo reserve:\n\tbook book_id\nTo issue:\n\tissue book_id\nTo return: \n\treturn book_id copy_id\nTo renew: \n\trenew book_id copy_id\n---------------------------------\n");
	printf("\nResponse sent-\n%s\n", response);
	return;
}

int main(int argc, char *argv[])
{
    struct sockaddr_in serv_addr, clnt_addr;

    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0) 
    {
        perror("Cannot open socket for connection\n");
        exit(1);
    }
    
    bzero((char *) &serv_addr, sizeof(serv_addr));
    portno = 5001;
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = inet_addr("127.0.0.1");
    serv_addr.sin_port = htons(portno);
 
    if (bind(sockfd, (struct sockaddr *) &serv_addr, sizeof(serv_addr)) < 0)
    {
         perror("Error - Cannot bind server on given port.\n");
         exit(1);
    }

    listen(sockfd,5);
    int clients = sizeof(clnt_addr);
    while(1)
    {
	bzero(buffer,1024);
        newsockfd = accept(sockfd, (struct sockaddr *) &clnt_addr, &clients);
        if (newsockfd<0)
        {
            perror("Error in accepting connection from clients..\n");
            exit(1);
        }
        int pid = fork();
        if (pid < 0)
        {
            perror("Error - Cannot fork process..\n");
	    exit(1);
        }
        else if(pid==0) {
		welcome();
		send(newsockfd, response, strlen(response), 0);
		while(1){
			show_all();
			bzero(buffer, 1024);
			buffer[0]='\0';
	    		int n = recv(newsockfd, buffer, 1024, 0);
	    		printf("Processing client %d, %d\n", inet_ntoa(clnt_addr.sin_addr.s_addr), (int)clnt_addr.sin_port);
	   	 	printf("Msg from client - %s\n", buffer);
			if(buffer[0]=='e'&&buffer[1]=='x'&&buffer[2]=='i'&&buffer[3]=='t'){
				printf("Client %d, %d exited!\n\n", inet_ntoa(clnt_addr.sin_addr.s_addr), (int)clnt_addr.sin_port);
				exit(1);
			}
	    		if(n<0){
				printf("Cannot read from socket. Possibly the client has disconnected..\n");
		        	close(sockfd);
				exit(1);
	    		}
			rcvd_strings[0][0]='\0';rcvd_strings[1][0]='\0';rcvd_strings[2][0]='\0';
			response[0]='\0';
			process_command(buffer);
			int res=-1;
			if(strcmp(rcvd_strings[0],"search")==0){
				if(strcmp(rcvd_strings[1],"name")==0){
					res = search(rcvd_strings[2],1);
				} else if(strcmp(rcvd_strings[1],"id")==0){
					res = search(rcvd_strings[2],2);
				} else {
					opac_error("Error! Command invalid..Please retry..\n");
				}
			} else if(strcmp(rcvd_strings[0],"add")==0){
				if(strcmp(rcvd_strings[1],"new")==0){
                                        res = add(rcvd_strings[2],1);
                                } else if(strcmp(rcvd_strings[1],"copy")==0){
                                        res = add(rcvd_strings[2],2);
                                } else {
                                        opac_error("Error! Command invalid..Please retry..\n");
                                }
			} else if(strcmp(rcvd_strings[0],"book")==0){
				res = atoi(rcvd_strings[1]);
				printf("res-%d\n", res);
				if(!(res>0 && res<=num_books)) opac_error("Error! book_id not a valid number. Please try again..\n");
				else res = reserve(res);
			} else if(strcmp(rcvd_strings[0],"issue")==0){
				res = atoi(rcvd_strings[1]);
                                if(!(res>0 && res<=num_books)) opac_error("Error! book_id not a valid number. Please try again..\n");
                                else res = issue(res);
			} else if(strcmp(rcvd_strings[0],"return")==0){
                                res = atoi(rcvd_strings[1]);
                                if(!(res>0 && res<=num_books)) opac_error("Error! book_id not a valid number. Please try again..\n");
                                else res = return_book(res, atoi(rcvd_strings[2]));
			} else if(strcmp(rcvd_strings[0],"renew")==0){
                                res = atoi(rcvd_strings[1]);
                                if(!(res>0 && res<=num_books)) opac_error("Error! book_id not a valid number. Please try again..\n");
                                else res = renew(res, atoi(rcvd_strings[2]));
			} else if(strcmp(rcvd_strings[0], "")==0){
				printf("Client exited! Exiting thread.\n");
				exit(1);
			} else{
				opac_error("Invalid request, cannot understand your command. Please retry..\n");
			}
			printf("\nResponse sent-\n%s\n", response);
	    		n = send(newsockfd, response, strlen(response), 0);
            		if(n<0){
                		printf("Cannot write to socket. Possibly the client has disconnected..\n");
                		exit(1);
            		}
        	}
	}
        else
        {
            close(newsockfd);
        }
    }
}
