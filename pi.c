#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <pthread.h>


long thread_count;
long long n;
int number_in_circle;
pthread_mutex_t mutex;
long perthread_iter;

void* simupi(){
	int incircle=0;
	double x;
	double y;
	double dist;
	unsigned int rand_state=rand();
	for(int i=0;i<perthread_iter;i++){
		x=rand_r(&rand_state)/((double)RAND_MAX)*2.0-1.0;
		y=rand_r(&rand_state)/((double)RAND_MAX)*2.0-1.0;
		dist=x*x+y*y;
		if(dist<=1)
			incircle++;
	}
	pthread_mutex_lock(&mutex);
	number_in_circle+=incircle;
	pthread_mutex_unlock(&mutex);
	return NULL;
}

int main(int argc, char *argv[]){

	srand(time(NULL));
	long thread;
	pthread_t* thread_handles;
	
	thread_count=strtol(argv[1],NULL,10);
	n=strtoll(argv[2],NULL,10);
	perthread_iter=n/thread_count;

	thread_handles=(pthread_t*)malloc(thread_count*sizeof(pthread_t));
	pthread_mutex_init(&mutex,NULL);
	number_in_circle=0;

	for(thread=0;thread<thread_count;thread++){
		pthread_create(&thread_handles[thread],NULL,simupi,(void*)thread);
	}
	for(thread=0;thread<thread_count;thread++){
		pthread_join(thread_handles[thread],NULL);
	}
	
	double pi;
	pi=4*number_in_circle/(double)n;
	printf("%f\n",pi);
	pthread_mutex_destroy(&mutex);
	free(thread_handles);
	return 0;
}




