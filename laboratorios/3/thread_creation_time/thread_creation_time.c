#include <stdio.h>
#include <time.h>
#include <pthread.h>
#include <errno.h>
#include <stdlib.h>

void * secondary_function(){
	return NULL;
}

int main(int argc, char* arg[]){
	if(argc==2){
		//variable declaration and initialization
		size_t amount_of_tests = 0;
		time_t lowest_time_taken,start_time,finish_time,time_taken = 0;
		size_t thread_count = amount_of_tests;
		pthread_t* threads = malloc((size_t)(thread_count * sizeof(pthread_t)));
		amount_of_tests = atoi(arg[1]);

		//repeat amount_of_tests times, find the lowest execution time
		for(size_t i = 0; i < amount_of_tests; i++){
			time(&start_time);
			pthread_create(&threads[i], NULL, secondary_function, (void*)i);
			time(&finish_time);
			time_taken = finish_time - start_time;
			if(i = 0){
				lowest_time_taken = time_taken;
			}else{
				if(lowest_time_taken > time_taken){
					lowest_time_taken = time_taken;
				} 
			}		
			pthread_join(threads[i], NULL);		
		}
		free(threads);
		printf("Minimum thread creation and destruction time was %lis among %zu trials\n", lowest_time_taken, amount_of_tests);
	}else{
		perror("Please type one integer in input arguments. Error code 1\n");
		return 2;
	}
	return 0;
}
