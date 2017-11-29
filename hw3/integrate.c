#include <stdio.h>
#include <math.h>
#include <mpi.h>
#define PI 3.1415926535

int main(int argc, char **argv) 
{
  long long i, num_intervals;
  double rect_width, area, sum, x_middle; 

  int my_rank;
  int p;
  double psum;

  sscanf(argv[1],"%11u",&num_intervals);
 
  MPI_Init(&argc,&argv);
  MPI_Comm_rank(MPI_COMM_WORLD,&my_rank);
  MPI_Comm_size(MPI_COMM_WORLD,&p);

  MPI_Bcast(&num_intervals,1,MPI_LONG_LONG,0,MPI_COMM_WORLD);

  rect_width = PI / num_intervals;
  sum=0;
  psum = 0;
  
  for(i = 1+my_rank; i < num_intervals + 1;i=i+p) {

    /* find the middle of the interval on the X-axis. */ 
    x_middle = (i - 0.5) * rect_width;
    area = sin(x_middle) * rect_width; 
    psum = psum + area;
  }

  MPI_Reduce(&psum,&sum,1,MPI_DOUBLE,MPI_SUM,0,MPI_COMM_WORLD);

  if(my_rank==0){
  	printf("The total area is: %f\n", (float)sum);
  }

  MPI_Finalize();
}   
