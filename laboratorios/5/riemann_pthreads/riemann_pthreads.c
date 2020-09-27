#include <stdio.h>
#include <errno.h>
#include <time.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>

typedef struct timespec walltime_t;

typedef struct{
    double lower_limit;
    double upper_limit;
    double number_of_rectangles;
    double rectangle_width;
    double combined_area;
    pthread_mutex_t mutex;
    double current_iteration;
} shared_data_t;

void walltime_start(walltime_t* start){
	clock_gettime(CLOCK_MONOTONIC, start);
}

double walltime_elapsed(const walltime_t* start){
	walltime_t finish;
	clock_gettime(CLOCK_MONOTONIC, &finish);
	double elapsed = (finish.tv_sec - start->tv_sec);
	elapsed += (finish.tv_nsec - start->tv_nsec) / 1000000000.0;
	return elapsed;
}

double f(double x){
	double result = 0;
	result = (x*x) + 1;
	return result;
}

double riemann(double lower_limit, double upper_limit, double number_of_rectangles){
	double rectangle_width =  (upper_limit-lower_limit)/number_of_rectangles;
	double combined_area = 0;
	for(int i = 0; i < number_of_rectangles; ++i){
		combined_area += f(lower_limit+(i*rectangle_width));	
	}
	return combined_area*rectangle_width;
}

void * riemann_parallel(void * args){
	shared_data_t * shared_data = (shared_data_t*) args;
	pthread_mutex_lock(&shared_data->mutex);
        printf("Combined area %lf=f(%lf+(%lf*%lf)\n", shared_data->combined_area, shared_data->lower_limit, shared_data->rectangle_width,shared_data->current_iteration);
	shared_data->combined_area+= f(shared_data->lower_limit+(shared_data->current_iteration*shared_data->rectangle_width));
	shared_data->current_iteration++;
	pthread_mutex_unlock(&shared_data->mutex);
	return NULL;
}

int main(int argument_counter, char * arguments[]){
	shared_data_t shared_data;
	int thread_count = 0;
	if(argument_counter >=5){
		shared_data.lower_limit = (int)strtoul(arguments[1], NULL, 10);
		shared_data.upper_limit = (int)strtoul(arguments[2], NULL, 10);
		shared_data.number_of_rectangles = (int) strtoul(arguments[3], NULL, 10);
		thread_count = (int) strtoul(arguments[4], NULL, 10);
	}else{
		perror("Please input the four arguments a, b, n and the number of threads");
		exit(1);
	}
	
	walltime_t time;
	shared_data.rectangle_width = (shared_data.upper_limit-shared_data.lower_limit)/shared_data.number_of_rectangles;
	pthread_t* threads = malloc((size_t)(thread_count * sizeof(pthread_t)));
	pthread_mutex_init(&shared_data.mutex, NULL);

	walltime_start(&time);

	double rectangles_per_thread;	
	rectangles_per_thread =  shared_data->number_of_rectangles/thread_count; 

	if(rectangles_per_thread < 1){  
		for (size_t i = 0; i < shared_data->number_of_rectangles; ++i) {
                	pthread_create(&threads[i], NULL, riemann_parallel, (void*)&shared_data);
        	}	
	}

	if(rectangles_per_thread == 1){
		for (size_t i = 0; i < thread_count; ++i) {
                	pthread_create(&threads[i], NULL, riemann_parallel, (void*)&shared_data);
        	}
	}

	if(rectangles_per_thread > 1){
		for(size_t i = 0; i < thread_count; i++){
			pthread_create(&threads[i], NULL, riemann_parallel, (void *)shared_data);
		}
	}
	
	for (size_t i = 0; i < thread_count; ++i) {
        	pthread_join(threads[i], NULL);
    	}

	printf("The result is = %f\n", shared_data.combined_area*shared_data.rectangle_width);
	double elapsed = walltime_elapsed(&time);
	printf("Time taken = %f\n", elapsed);

	free(threads);
	pthread_mutex_destroy(&shared_data.mutex);
	return 0;
}
