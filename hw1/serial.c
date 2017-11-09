#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <pthread.h>
unsigned int seed=0;
double randf(double low, double high){
	        return(rand_r(&seed)/(double)(RAND_MAX))*abs(low-high)+low;
}

int main(int argc, char *argv[]){
	srand(time(NULL));
	int number_incircle=0;
	double x;
	double y;
	double dist;
	long long n=strtoll(argv[1],NULL,10);
	for(int i=0;i<n;i++){
		x=randf(-1,1);
		y=randf(-1,1);					
		dist=x*x+y*y;
	
		if(dist<=1)

    			number_incircle++;
	}
	double pi;
	pi=4*number_incircle/((double)n);
	printf("%f\n",pi);
	return 0;	
}
