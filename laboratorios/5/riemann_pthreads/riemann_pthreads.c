#include <stdio.h>
#include <errno.h>
#include <time.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <math.h>

typedef struct timespec walltime_t;

typedef struct
{

    double lower_limit;
    double upper_limit;
    double number_of_rectangles;
    double rectangle_width;
    double combined_area;
    size_t current_iteration;
    size_t iterations_per_thread;
    pthread_mutex_t mutex;

} shared_data_t;

typedef struct 
{

    size_t thread_num;
    size_t thread_counter;
    double thread_lower_limit;
    double calculated_area;
    shared_data_t* shared_data;

} thread_data_t;

void walltime_start(walltime_t* start)
{

	clock_gettime(CLOCK_MONOTONIC, start);

}

double walltime_elapsed(const walltime_t* start)
{
	walltime_t finish;
	clock_gettime(CLOCK_MONOTONIC, &finish);
	double elapsed = (finish.tv_sec - start->tv_sec);
	elapsed += (finish.tv_nsec - start->tv_nsec) / 1000000000.0;
	return elapsed;
}

//f(x)
double f(double x)
{

	double result = 0;
	result = (x*x) + 1;
	return result;

}

//Serial implementation of Riemann
double riemann(double lower_limit, double upper_limit, double number_of_rectangles)
{

	double rectangle_width =  (upper_limit-lower_limit)/number_of_rectangles;
	double combined_area = 0;

	for(size_t i = 0; i < number_of_rectangles; ++i){
		combined_area += f(lower_limit+(i*rectangle_width));	
	}
	return combined_area*rectangle_width;

}

//Concurrent implementation of Riemann
void * riemann_parallel(void * args)
{

	thread_data_t* thread_data = (thread_data_t*) args;
	shared_data_t* shared_data = thread_data->shared_data;

	for(thread_data->thread_counter = 0; thread_data->thread_counter < shared_data->iterations_per_thread; thread_data->thread_counter++)
	{


		thread_data->calculated_area += f(thread_data->thread_lower_limit+(thread_data->thread_counter*shared_data->rectangle_width));
		printf("Current iteration: %zu\n", thread_data->thread_counter);
		printf("Thread %zu calculated the following area = (%lf) = f( %lf + ( %zu * %lf  )\n",	thread_data->thread_num, 
		thread_data->calculated_area, thread_data->thread_lower_limit, thread_data->thread_counter, shared_data->rectangle_width);


	}


	pthread_mutex_lock(&shared_data->mutex);
	shared_data->combined_area += thread_data->calculated_area;
	pthread_mutex_unlock(&shared_data->mutex);

	return NULL;

}

void join_threads(pthread_t * threads, size_t thread_count){

        for (size_t i = 0; i < thread_count; ++i)
        {
                pthread_join(threads[i], NULL);
        }

}

void free_memory(pthread_t * threads, shared_data_t * shared_data, thread_data_t * thread_data_list){
        free(threads);
        free(shared_data);
        free(thread_data_list);
        pthread_mutex_destroy(&shared_data->mutex);
}

int main(int argc, char * args[])
{

	walltime_t time;	
	shared_data_t * shared_data 	= (shared_data_t*)calloc(1, sizeof(shared_data_t));
	//size_t thread_count = 1;
        //size_t thread_count = 16;
	//size_t thread_count = 32;
	size_t thread_count = 64;
	printf("Threads available = %zu\n", thread_count);
		
	if(argc >= 3)
	{
		shared_data->lower_limit 		= (double) strtoul(args[1], NULL, 10);
		shared_data->upper_limit 		= (double) strtoul(args[2], NULL, 10);
		shared_data->number_of_rectangles 	= (double) strtoul(args[3], NULL, 10);
		printf("Lower limit recieved: %lf\n", shared_data->lower_limit);
		printf("Upper limit recieved: %lf\n", shared_data->upper_limit);
		printf("Number of rectangles recieved: %lf\n", shared_data->number_of_rectangles);
	}
	else
	{
		printf("Error: not enough input values. Please insert 3 input values (a,b,n)\n");
		exit(1);
	}

	shared_data->rectangle_width = (shared_data->upper_limit-shared_data->lower_limit)/shared_data->number_of_rectangles;
	printf("Rectangle width calculated: %lf\n", shared_data->rectangle_width);
	pthread_t* threads 		= malloc((size_t)(thread_count * sizeof(pthread_t)));
	thread_data_t* thread_data_list = malloc((size_t)(thread_count * sizeof(thread_data_t)));

	thread_data_list->shared_data = shared_data;

	pthread_mutex_init( &shared_data->mutex, NULL);

	//time starts running
	walltime_start(&time);

	//Check how many rectangles should be calcualted per thread
	double rectangles_per_thread = shared_data->number_of_rectangles/thread_count;
	printf("Rectangles per thread calculated: %lf\n", rectangles_per_thread);

	//Case 1: More threads than rectangles-Create n threads where n is the number of rectangles, only one iteration per thread
	if(rectangles_per_thread <= 1)
	{
		
		printf("Entered case 1: rectangles per thread  <=  1\n");
		shared_data->iterations_per_thread = 1;
		printf("Iterations per thread: %zu\n", shared_data->iterations_per_thread);
		printf("Casted number of rectangles = %zu \n", (size_t) shared_data->number_of_rectangles);

		for (size_t i = 0; i  <  (size_t) shared_data->number_of_rectangles ; ++i) 
		{
				
			thread_data_list[i].shared_data = shared_data;
			thread_data_list[i].thread_lower_limit = shared_data->lower_limit + (shared_data->rectangle_width*i);
			thread_data_list[i].thread_num = i;
			printf("LOWER LIMIT SENT TO THREAD = %lf\n", thread_data_list[i].thread_lower_limit);
                	pthread_create(&threads[i], NULL, riemann_parallel, (void*)&thread_data_list[i]);
			printf("Thread %zu created\n", i);

        	}	

	}
	

	//Case 2: More than one rectangle per thread-Create all threads available
	if(rectangles_per_thread > 1)
	{

		printf("Entered case 2: rectangles per thread > 1\n");
		//Case 2.1 If rectangles_per_thread is a whole number, divide equally among all available threads
		if(floor(rectangles_per_thread) == rectangles_per_thread)
		{

			printf("Entered case 2.1: whole number of rectangles per thread\n");
			shared_data->iterations_per_thread = rectangles_per_thread;
			printf("Iterations per thread: %zu\n", shared_data->iterations_per_thread);

			for(size_t i = 0; i < thread_count; ++i)
			{

				thread_data_list[i].shared_data = shared_data;
                        	thread_data_list[i].thread_lower_limit = shared_data->lower_limit + (shared_data->rectangle_width*i);
                        	thread_data_list[i].thread_num = i;
                        	printf("LOWER LIMIT SENT TO THREAD = %lf\n", thread_data_list[i].thread_lower_limit);
                        	pthread_create(&threads[i], NULL, riemann_parallel, (void*)&thread_data_list[i]);
                        	printf("Thread %zu created\n", i);

			}

		}

		//Case 2.2 If rectangles_per_thread is not a whole number, divide rectangles equally among all except one available threads, 
		//then load the last thread with all remaining rectangles
		else
		{
			printf("Entered case 2.2: non whole number of rectangles per thread\n");
			shared_data->iterations_per_thread = floor(rectangles_per_thread);
			printf("Iterations per thread: %zu for thread_count-1: %zu\n", shared_data->iterations_per_thread, thread_count-1);

			for(size_t i = 0; i < thread_count-1; ++i)
			{

			        thread_data_list[i].shared_data = shared_data;
                        	thread_data_list[i].thread_lower_limit = shared_data->lower_limit + (shared_data->rectangle_width*i);
                                thread_data_list[i].thread_num = i;
                                printf("LOWER LIMIT SENT TO THREAD = %lf\n", thread_data_list[i].thread_lower_limit);
                        	pthread_create(&threads[i], NULL, riemann_parallel, (void*)&thread_data_list[i]);
                        	printf("Thread %zu created\n", i);

			}
			printf("Thread count-1 completed %zu iterations\n",shared_data->iterations_per_thread*thread_count-1);

			//Load the last thread with any remaining rectangles
			
			shared_data->iterations_per_thread = shared_data->number_of_rectangles-(floor(rectangles_per_thread)*thread_count-1);
			thread_data_list[thread_count-1].shared_data = shared_data;
			thread_data_list[thread_count-1].thread_lower_limit = shared_data->lower_limit + (shared_data->rectangle_width*thread_count-1);
			thread_data_list[thread_count].thread_num = thread_count-1;
                        printf("LOWER LIMIT SENT TO THREAD = %lf\n", thread_data_list[thread_count].thread_lower_limit);
			pthread_create(&threads[thread_count], NULL, riemann_parallel, (void *)shared_data);
			printf("Final thread %zu created, loaded with %zu iterations\n", thread_data_list[thread_count].thread_num, 
			shared_data->iterations_per_thread);
		}
	}

	join_threads(threads, thread_count);

	printf("Combined area = %lf  = %lf*%lf\n",shared_data->combined_area*shared_data->rectangle_width,shared_data->rectangle_width,shared_data->combined_area);
	double elapsed = walltime_elapsed(&time);
	printf("Time taken = %f\n", elapsed);

	free_memory(threads,shared_data,thread_data_list);

	return 0;
}
