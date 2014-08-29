#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <mpi.h>

int P=10, sqrtP=3;
int myrank;
int rowsForAll, size;

int max(int a, int b){
	return ((a>b) ? a : b);
}

void multiply_matrix(int **matrix1, int **matrix2, int **product){

	/*printf("Hello from process %d\n", myrank);*/
	int i,j;
	/*if(myrank==2){
		printf("Hello from process %d, rows-%d, size-%d\n", myrank, rowsForAll, size);
		for(i=0;i<rowsForAll;i++){
			for(j=0;j<size;j++){
				printf("%d ", matrix1[i][j]);
			}
			printf("\n");
		}
		for(i=0;i<size;i++){
			for(j=0;j<rowsForAll;j++){
				printf("%d ", matrix2[i][j]);
			}
			printf("\n");
		}
	}*/
	int c,d,k,sum=0;
    for(c=0;c<rowsForAll;c++){
		for(d=0;d<size;d++){
	        for(k=0;k<size;k++){
	        	sum = sum + matrix1[c][k]*matrix2[k][d];
      		}
        	product[c][d] = sum;
        	sum = 0;
      	}
    }
    /*if(myrank==2){
	    printf("\n\nReturn-\n");
	    for(i=0;i<rowsForAll;i++){
				for(j=0;j<rowsForAll;j++){
					printf("%d ", product[i][j]);
				}
				printf("\n");
		}
	}*/
	return;
}

int main(int argc, char **argv){
	
	int i, j;
	MPI_Init(&argc, &argv);
	MPI_Status status;
	MPI_Comm_size(MPI_COMM_WORLD,&P);
	sqrtP = sqrt((int)(P-1));
	MPI_Comm_rank(MPI_COMM_WORLD,&myrank);
	//printf("Hello World, my rank is %d in the group of %d processes\n", myrank, P);
	if(myrank==0) printf("Enter 4 integers as size of matrices - \n");
	
	MPI_Barrier(MPI_COMM_WORLD);

	if(myrank==0){
		
		int m1, n1, m2, n2;
		scanf(" %d %d %d %d", &m1, &n1, &m2, &n2);
		size = max(max(m1,n1),max(m2,n2));
		while(size%sqrtP != 0){
			size++;
		}

		int **matrix1, **matrix2;

		matrix1 = (int **)malloc(sizeof(int*)*size);
		matrix2 = (int **)malloc(sizeof(int*)*size);
		for(i=0;i<size;i++){
			matrix1[i] = (int *)malloc(sizeof(int)*size);
			matrix2[i] = (int *)malloc(sizeof(int)*size);
		}

		for(i=0;i<size;i++){
			for(j=0;j<size;j++){
				matrix1[i][j] = matrix2[i][j] = 0;
			}
		}

		if(n1!=m2){
			printf("\n\nThese two matrices cannot be multiplied.n1!=m2\n\n");
			return 0;
		}
		/**Read matrices**/
		printf("\n\nEnter matrix 1 - %d\n",sqrtP);
		for(i=0;i<m1;i++){
			for(j=0;j<n1;j++){
				scanf("%d", &matrix1[i][j]);
			}
		}
		printf("\n\nEnter matrix 2 - \n");
		for(i=0;i<m2;i++){
			for(j=0;j<n2;j++){
				scanf("%d", &matrix2[i][j]);
			}
		}

		/**Print matrices**/
		
		/*printf("Matrix-1\n");
		for(i=0;i<size;i++){
			for(j=0;j<size;j++){
				printf("%d ", matrix1[i][j]);
			}
			printf("\n");
		}
		printf("Matrix-2\n");
		for(i=0;i<size;i++){
			for(j=0;j<size;j++){
				printf("%d ", matrix2[i][j]);
			}
			printf("\n");
		}
		*/
		rowsForAll = (size/sqrtP);

		printf("Sending-\n");
		for(i=1;i<P;i++){
			MPI_Send(&rowsForAll, 1, MPI_INT, i, 0, MPI_COMM_WORLD);
			MPI_Send(&size, 1, MPI_INT, i, 1, MPI_COMM_WORLD);
		}
		
		int k,processor=1;
		
		for(i=0;i<sqrtP;i++){
			for(j=0;j<sqrtP;j++){
				int l = (int)((processor-1)/sqrtP);
				for(k=l;k<l+rowsForAll;k++){
					MPI_Send(&matrix1[k][0], size, MPI_INT, processor, 2, MPI_COMM_WORLD);
				}
				
				l = (int)((processor-1)%sqrtP)*rowsForAll;
				int t;
				
				for(t=0;t<size;t++){
					int *temp = (int *)malloc(sizeof(int)*rowsForAll);
					for(k=0;k<rowsForAll;k++){
						temp[k] = matrix2[t][k+l];				
					}
					MPI_Send(&temp[0], rowsForAll, MPI_INT, processor, 2, MPI_COMM_WORLD);
				}
				processor++;
			}
		}

		printf("Rows sent, Start receiving..\n");
		
		int product[size][size];
		
		processor = 1;
		//MPI_Recv(&matrix2[j][0], rowsForAll, MPI_INT, 0, 2, MPI_COMM_WORLD, &status);
		for(i=0;i<sqrtP;i++){
			for(j=0;j<sqrtP;j++){
				int l = (int)((processor-1)/sqrtP);
				int t = (int)((processor-1)%sqrtP)*rowsForAll;
				for(k=l;k<l+rowsForAll;k++){
					MPI_Recv(&product[k][t], rowsForAll, MPI_INT, processor, 3, MPI_COMM_WORLD, &status);
				}
				processor++;
			}
		}

		printf("Product-\n");
		for(i=0;i<m1;i++){
			for(j=0;j<n2;j++){
				printf("%d ", product[i][j]);
			}
			printf("\n");
		}

	} else {

		MPI_Recv(&rowsForAll, 1, MPI_INT, 0, 0, MPI_COMM_WORLD, &status);
		MPI_Recv(&size, 1, MPI_INT, 0, 1, MPI_COMM_WORLD, &status);
		//printf("rowsForAll rcvd - %d, process - %d\n", rowsForAll, myrank);
		int **matrix1, **matrix2;
		matrix1 = (int **)malloc(sizeof(int*)*rowsForAll);
		matrix2 = (int **)malloc(sizeof(int*)*size);
		for(i=0;i<rowsForAll;i++){
			matrix1[i] = (int *)malloc(sizeof(int)*size);
		}
		for(i=0;i<size;i++){
			matrix2[i] = (int *)malloc(sizeof(int)*rowsForAll);
		}

		int **product;
		product = (int **) malloc (sizeof(int *)*rowsForAll);
		for(i=0;i<rowsForAll;i++){
			product[i] = (int *) malloc (sizeof(int)*rowsForAll);
		}
		for(i=0;i<rowsForAll;i++){			
			MPI_Recv(&matrix1[i][0], size, MPI_INT, 0, 2, MPI_COMM_WORLD, &status);
		}
		for(j=0;j<size;j++){
			MPI_Recv(&matrix2[j][0], rowsForAll, MPI_INT, 0, 2, MPI_COMM_WORLD, &status);
		}
		/*if(myrank==1){
		for(i=0;i<size;i++){
			for(j=0;j<rowsForAll;j++){
				printf("%d ", matrix2[i][j]);
			}
			printf("\n");
		}}*/

		multiply_matrix(matrix1, matrix2, product);
		
		for(i=0;i<rowsForAll;i++){			
			MPI_Send(&product[i][0], rowsForAll, MPI_INT, 0, 3, MPI_COMM_WORLD);
		}
		//printf("I have finished my work. - %d\n", myrank);

	}

	MPI_Finalize();
	return 0;
		
}
