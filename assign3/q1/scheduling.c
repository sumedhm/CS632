#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <mpi.h>
#define LENGTH 50
#define ARG_SIZE 6
#define file "a"
#define CITY1 0
#define CITY2 1
#define k 2

int P, token, incity, clock, time, x, FINISH = 0, FLAG = 0;
//FLAG tells whether all previous processes have exited or not.
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
	arg[4] -> FINISH. If a process exits, it informs its neighbor to change its neighbor. A process will exit only if all others lesser have exited.
	arg[5] -> Lowest rank active process. This needs to be passed so that process (P-1) can change its nextrank if 0 exits.
	*/

	/*** We will assume it takes 5 unit time to pass the bridge ***/

	FINISH = 0;
	while(slots[x]!='.'&&x<LENGTH){			//Everyone reads their first schedule.
		if(slots[x]=='\0'){
			FINISH = 1; break;
		}
		time = time*10+atoi(&slots[x]);
        	x++;
        }
        x++;
	if(myrank==0){
		printf("Process 0 initiating..\n\n");
		FLAG = 1; //process 0 will be first to exit.
		prevrank = P-1;
		arg[0] = token = 1;
		arg[1] = clock;
		arg[2] = incity;
		arg[3] = 0;
		arg[4] = FINISH;
		arg[5] = 0;
		clock++;
		if(clock>=time){
			//printf("Process - %d, token - %d, clock - %d, city - %d..\n", myrank, token, clock, incity);
			int newcity = (incity+1)%2;
			printf("City%d====0====>city%d..\n", incity, newcity);
			incity = newcity;
			token = 0;
			clock+=4; //5 units will be added in total, 4 here, 1 below. As a car takes 5 units to cross bridge.
			MPI_Send(&arg, ARG_SIZE, MPI_INT, 1, 0, MPI_COMM_WORLD);
		} else {
			MPI_Send(&arg, ARG_SIZE, MPI_INT, 1, 0, MPI_COMM_WORLD);
		}
		clock++;
	}
	while(1){
                //printf("Process - %d, token - %d, clock - %d, time - %d, city - %d, cars - %d, nextrank - %d, prevrank - %d..\n", myrank, token, clock, time, incity, arg[3], nextrank, prevrank);
		MPI_Recv(&arg, ARG_SIZE, MPI_INT, prevrank, 0, MPI_COMM_WORLD, &status);
		//printf("received by %d..\n\n", myrank);
		token = arg[0]; clock = max(clock, arg[1])+1; //clock synchronization.
		clock++;
		FLAG = arg[4];
		if(FLAG){
			prevrank = P-1;
		}
		if(myrank == P-1){
			nextrank = arg[5]; //update nextrank.
		}
		if(FINISH && (FLAG||myrank==0)){	//0 can exit anytime and others can exit if FLAG is 1.
			arg[4] = 1;
			arg[5] = nextrank;
			MPI_Send(&arg, ARG_SIZE, MPI_INT, nextrank, 0, MPI_COMM_WORLD);
			break;
		} else {
			arg[4] = 0;
		}
		if(token==1 && !FINISH){
			int newcity = (incity+1)%2;
			arg[0] = 1; arg[1] = clock;
			if(incity==arg[2]&&arg[3]<k){
				if(clock>=time){
					//Travel to other city. arg[3] tells cars are less than k on bridge. incity ensures we are in the same city as the previous car. This ensures we will travel in same direction.
					//printf("Y1 - %d\n",myrank);
					//printf("Process - %d, token - %d, clock - %d, time - %d, city - %d, cars - %d, nextrank - %d, prevrank - %d, FLAG - %d..\n", myrank, token, clock, time, incity, arg[3], nextrank, prevrank, FLAG);
					printf("City%d====%d====>city%d..Clock - %d..\n", incity, myrank, newcity, clock);
					incity = newcity;
					arg[3]++; arg[2] = incity;
					MPI_Send(&arg, ARG_SIZE, MPI_INT, nextrank, 0,MPI_COMM_WORLD);
					clock+=4; //Time taken to travel.
					time = 0;
					if(slots[x]=='\0'){
						FINISH = 1;
					} else {
                				while(slots[x]!='.'&&x<LENGTH){
                        				time = time*10+atoi(&slots[x]);
                        				x++;
                				}
                				x++;
					}
				} else {
					//printf("Y2 - %d\n",myrank);
					//Forward token to nextrank in same city. no change in arg[5,2,3]. arg[2] & arg[3] will be updated next time.
					MPI_Send(&arg, ARG_SIZE, MPI_INT, nextrank, 0, MPI_COMM_WORLD);
				}
			} else if(incity!=arg[2] || arg[3]>=k){ //we are on different side of city than the car on bridge or already k cars on bridge.
				arg[3]=0;	//restart from 1 car on bridge.
				clock+=4;	//no action for 5 seconds since the sender is on the bridge, and no one else can go on bridge.
				arg[1] = clock;
				if(clock>=time){
					//printf("Y3 - %d\n", myrank);
                                        //Travel to other city. arg[3] tells cars are less than k on bridge. incity ensures we are in the same city as the previous car. This ensures we will travel in same direction.
					//printf("Process - %d, token - %d, clock - %d, time - %d, city - %d, cars - %d, nextrank - %d, prevrank - %d, FLAG - %d..\n", myrank, token, clock, time, incity, arg[3], nextrank, prevrank, FLAG);
                                        printf("City%d====%d====>city%d..Clock - %d..\n", incity, myrank, newcity, clock);
                                        incity = newcity;
                                        arg[3]++; arg[2] = incity;
					MPI_Send(&arg, ARG_SIZE, MPI_INT, nextrank, 0, MPI_COMM_WORLD);
                                        clock+=4; //Time taken to travel.
                                        time = 0;
					if(slots[x]=='\0'){
						FINISH = 1;
					} else {
	                                        while(slots[x]!='.'&&x<LENGTH){
        	                                        time = time*10+atoi(&slots[x]);
                	                                x++;
                        	                }
					}
                                        x++;
                                } else {
                                        //Forward token to nextrank in same city. no change in arg[5,2,3]. arg[2] & arg[3] will be updated next time.
					//printf("Y4 - %d\n",myrank);
					MPI_Send(&arg, ARG_SIZE, MPI_INT, nextrank, 0, MPI_COMM_WORLD);
                                }
			} else {
				printf("ERROR - Process - %d, token - %d, clock - %d, time - %d, city - %d, cars - %d..\n", myrank, token, clock, time, incity, arg[3]);
			}
			token=0;
		} else {
//			printf("Finished->%d...\n", myrank);
			MPI_Send(&arg, ARG_SIZE, MPI_INT, nextrank, 0, MPI_COMM_WORLD);
		}
	}

	MPI_Finalize();
	return 0;
		
}
