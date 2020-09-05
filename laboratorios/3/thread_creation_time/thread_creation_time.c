#include <stdio.h>
#include <time.h>
#include <pthread.h>
#include <errno.h>
#include <stdlib.h>

struct timespec start, finish;

void * secondary_function(void* args){
	return NULL;
}

int main(int argc, char* arg[]){
	size_t thread_count = 0;
	if(argc>=2){
		thread_count = (size_t)strtoul(arg[1], NULL, 10);
	}else{
		perror("Please type one integer in input arguments.\n");
		exit(1);
	}

	double lowest_time_taken = 0;
        pthread_t* threads = malloc((size_t)(thread_count * sizeof(pthread_t)));
        for(size_t i = 0; i < thread_count; i++){
		clock_gettime(CLOCK_REALTIME, &start);
                pthread_create(&threads[i], NULL, secondary_function, (void*)i);
		clock_gettime(CLOCK_REALTIME, &finish);
                unsigned int time_taken = finish.tv_nsec - start.tv_nsec;
                if(i == 0){
                	lowest_time_taken = time_taken;
                }else if(lowest_time_taken > time_taken){
			lowest_time_taken = time_taken;
		}
        }

	for(size_t j = 0; j< thread_count; j++){
		pthread_join(threads[j], NULL);	
	}

        free(threads);
        printf("Minimum thread creation and destruction time was %lfs among %zu trials\n", lowest_time_taken/1000000000, thread_count);
	return 0;
}
