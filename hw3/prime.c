#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include<mpi.h>

int isprime(int n) {
  int i,squareroot;
  if (n>10) {
    squareroot = (int) sqrt(n);
    for (i=3; i<=squareroot; i=i+2)
      if ((n%i)==0)
        return 0;
    return 1;
  }
  else
    return 0;
}

int main(int argc, char *argv[])
{
  int pc,       /* prime counter */
      foundone; /* most recent prime found */
  long long int n, limit;
  
  long long int max_p;
  int my_rank;
  int p;  
  int sumpc;
  int step;
  
 
  sscanf(argv[1],"%llu",&limit);	
  
  MPI_Init(&argc,&argv);
  MPI_Comm_rank(MPI_COMM_WORLD,&my_rank);
  MPI_Comm_size(MPI_COMM_WORLD,&p);

  pc=0;
  foundone=0;
  step=p*2;
  int start;
  start=(my_rank*2)+1;

  if(my_rank>0){
 
  	for (n=start; n<=limit; n=n+step) {
    		if (isprime(n)) {
      			pc++;
      			foundone = n;
    		}			
  	}

  	MPI_Reduce(&pc,&sumpc,1,MPI_INT,MPI_SUM,0,MPI_COMM_WORLD);
  	MPI_Reduce(&foundone,&max_p,1,MPI_INT,MPI_MAX,0,MPI_COMM_WORLD);
  }	
  else{
        printf("Starting. Numbers to be scanned= %11d\n",limit);
  	pc=4;     /* Assume (2,3,5,7) are counted here */

  	for (n=start; n<=limit; n=n+step) {
    		if (isprime(n)) {
      			pc++;
      			foundone = n;
    		}			
  	}
	
	MPI_Reduce(&pc,&sumpc,1,MPI_INT,MPI_SUM,0,MPI_COMM_WORLD);
  	MPI_Reduce(&foundone,&max_p,1,MPI_INT,MPI_MAX,0,MPI_COMM_WORLD);

  	printf("Done. Largest prime is %d Total primes %d\n",max_p,sumpc);  
 
  }

  MPI_Finalize();

} 
