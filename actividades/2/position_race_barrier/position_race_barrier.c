#include <stdio.h>
#include <pthread.h>
#include <errno.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>
#include <time.h>

typedef struct {
    size_t finish_position;
    size_t thread_counter;
} shared_data_t;

shared_data_t data;
pthread_mutex_t lock;
pthread_barrier_t barrier;


void random_sleep(useconds_t min_milliseconds, useconds_t max_milliseconds){
        useconds_t duration = min_milliseconds;
        useconds_t range = max_milliseconds - min_milliseconds;
        if ( range > 0 )
                duration += rand() % range;
        usleep( 1000 * duration );
}


void * run_thread(void * args){
	size_t thread_number = (size_t) args;	
	random_sleep(1, 2);
	printf("Thread %zu/%zu: I am ready!\n", thread_number+1, data.thread_counter);
	pthread_barrier_wait(&barrier);
	pthread_mutex_lock(&lock);      
	printf("Thread %zu/%zu: I arrived at position %zu\n", thread_number+1, data.thread_counter, data.finish_position+1);
	data.finish_position++;
	pthread_mutex_unlock(&lock);
	return NULL;
}

int main(int argc, char * args[]){

    	size_t thread_count = 0;

    	if (argc >= 2) {
        	thread_count = (size_t)strtoul(args[1], NULL, 10);
		data.thread_counter = thread_count;
    	}else{
        	fprintf(stderr, "Error, invalid number of parameters\n");
        	return 1;
    	}

	srand( time(NULL) );
	//allocate memory and prepare mutex
    	pthread_t* threads = malloc((size_t)(thread_count * sizeof(pthread_t)));
	size_t value = thread_count;
	pthread_barrier_init(&barrier, /*attr*/ NULL, value);

	pthread_mutex_init(&lock, NULL);
	for (size_t i = 0; i < thread_count; ++i) {
        	pthread_create(&threads[i], NULL, run_thread, (void*)i);
    	}


	for (size_t i = 0; i < thread_count; ++i) {
        	pthread_join(threads[i], NULL);
    	}
        free(threads);
	pthread_mutex_destroy(&lock);
	pthread_barrier_destroy(&barrier);
    return 0;
}

