#include <stdio.h>
#include <pthread.h>
#include <errno.h>
#include <stdlib.h>

typedef struct {
    size_t finish_position;
    size_t thread_counter;
} shared_data_t;

shared_data_t data;
pthread_mutex_t lock;

void * run_thread(void * args){
	size_t thread_number = (size_t) args;	
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

	//allocate memory and prepare mutex
    	pthread_t* threads = malloc((size_t)(thread_count * sizeof(pthread_t)));
	pthread_mutex_init(&lock, NULL);

	for (size_t i = 0; i < thread_count; ++i) {
        	pthread_create(&threads[i], NULL, run_thread, (void*)i);
    	}

	for (size_t i = 0; i < thread_count; ++i) {
        	pthread_join(threads[i], NULL);
    	}
        free(threads);
	pthread_mutex_destroy(&lock);
    return 0;
}


