#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <mpi.h>
#define LENGTH 6
#define ARG_SIZE 4
#define file "a"
#define CITY1 1
#define CITY2 2
#define k 1

int P, token, incity, clock, time, x;
int myrank, prevrank, nextrank;
char slots[LENGTH];

int max(int a, int b){
	if(a>b) return a;
	else return b;
}

int main(int argc, char **argv){
	
	int i, j;
	MPI_Init(&argc, &argv);
	
	MPI_Status status;
	MPI_Comm_size(MPI_COMM_WORLD,&P);
	MPI_Comm_rank(MPI_COMM_WORLD,&myrank);
	token = 0;
	clock = 0;
	x = 0; //x will keep note of which time to take next from slots.
	if(myrank<P/2) incity = CITY1;
	else incity = CITY2;
	FILE *fp = fopen(file, "r");
	int count = 0;
	if(file!=NULL){
		while(fgets(slots, sizeof(slots),fp)){
			if(count==myrank){
				break;	
			} else count++;
		}
		fclose(fp);
	} else{
		printf("\n\nFile does not exist to get time slots. Exiting..\n");
		exit(1);
	}

	MPI_Barrier(MPI_COMM_WORLD);
	int arg[ARG_SIZE-1];
	prevrank = myrank-1;
	nextrank = myrank+1;
	if(myrank==P-1) nextrank=0;
	/*
	Array to be passed
	arg[0] -> token
	arg[1] -> clock(sender's)
	arg[2] -> where the token sender was, city1 or city2.
	arg[3] -> how many cars are currently on the bridge.
	*/

	/*** We will assume it takes 5 unit time to pass the bridge ***/
int f=0;
while(1){
	time=0;
	while(slots[x]!='.'&&x<LENGTH){			//Everyone reads their first schedule.
		if(slots[x]=='\0'){
		f=1;	break;
		}
		time = time+atoi(&slots[x]);
        	x++;
        }
	if(f==1) break;
        x++;
	
}
	printf("process - %d, time - %d\n", myrank, time);
/*
	if(myrank==0){
		prevrank = P-1;
		arg[0] = token = 1;
		arg[1] = clock;
		arg[2] = incity;
		arg[3] = 0;
		time = 0;
		while(slots[x]!='.'&&x<LENGTH){
			time = time+atoi(slots[x]);
			x++;
		}
		x++;
		if(clock>=time){
			printf("Process - %d, token - %d, clock - %d, city - %d..\n", myrank, token, clock, incity);
			int newcity = (incity+1)%2;
			printf("City%d====myrank====>city%d..\n", incity, newcity);
			incity = newcity;
			token = 0;
			clock+=4; //5 units will be added in total, 4 here, 1 below. As a car takes 5 units to cross bridge.
			MPI_Send(&arg, ARG_SIZE, MPI_INT, 0, 1, MPI_COMM_WORLD);
		}
		clock++;
	}

	while(1){
		MPI_Recv(&arg, ARG_SIZE, MPI_INT, 0, prevrank, MPI_COMM_WORLD, &status);
		token = arg[0]; clock = max(clock, arg[1])+1; //clock synchronization.
		clock++;
		if(token==1){
			int newcity = (incity+1)%2;
			arg[0] = 1; arg[1] = clock;
			if(incity==arg[2]&&arg[3]<k){
				if(clock>=time){
					//Travel to other city. arg[3] tells cars are less than k on bridge. incity ensures we are in the same city as the previous car. This ensures we will travel in same direction.
					printf("Process - %d, token - %d, clock - %d, time - %d, city - %d, cars - %d..\n", myrank, token, clock, time, incity, arg[3]);
					printf("City%d====myrank====>city%d..\n", incity, newcity);
					incity = newcity;
					arg[3]++; arg[2] = incity;
					MPI_Send(&arg, ARG_SIZE, MPI_INT, 0, nextrank, MPI_COMM_WORLD);
					clock+=4; //Time taken to travel.
					time = 0;
                			while(slots[x]!='.'&&x<LENGTH){
                        			time = time+atoi(slots[x]);
                        			x++;
                			}
                			x++;
				} else {
					//Forward token to nextrank in same city. no change in arg[5,2,3]. arg[2] & arg[3] will be updated next time.
					MPI_Send(&arg, ARG_SIZE, MPI_INT, 0, nextrank, MPI_COMM_WORLD);
				}
			} else if(incity!=arg[4] || arg[3]>=k){ //we are on different side of city than the car on bridge or already k cars on bridge.
				arg[3]=0;	//restart from 1 car on bridge.
				clock+=4;	//no action for 5 seconds since the sender is on the bridge, and no one else can go on bridge.
				if(clock>=time){
                                        //Travel to other city. arg[3] tells cars are less than k on bridge. incity ensures we are in the same city as the previous car. This ensures we will travel in same direction.
                                        printf("Process - %d, token - %d, clock - %d, time - %d, city - %d, cars - %d..\n", myrank, token, clock, time, incity, arg[3]);
                                        printf("City%d====myrank====>city%d..\n", incity, newcity);
                                        incity = newcity;
                                        arg[3]++; arg[2] = incity;
					MPI_Send(&arg, ARG_SIZE, MPI_INT, 0, nextrank, MPI_COMM_WORLD);
                                        clock+=4; //Time taken to travel.
                                        time = 0;
                                        while(slots[x]!='.'&&x<LENGTH){
                                                time = time+atoi(slots[x]);
                                                x++;
                                        }
                                        x++;
                                } else {
                                        //Forward token to nextrank in same city. no change in arg[5,2,3]. arg[2] & arg[3] will be updated next time.
					MPI_Send(&arg, ARG_SIZE, MPI_INT, 0, nextrank, MPI_COMM_WORLD);
                                }
			} else {
				printf("Error..\n");
				printf("Process - %d, token - %d, clock - %d, time - %d, city - %d, cars - %d..\n", myrank, token, clock, time, incity, arg[3]);
			}
			token=0;
		}
	}

*/		
	MPI_Finalize();
	return 0;
}
