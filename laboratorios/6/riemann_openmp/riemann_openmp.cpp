#include <stdio.h>
#include <errno.h>
#include <time.h>
#include <stdlib.h>
#include <unistd.h>
#include <omp.h>

typedef struct timespec walltime_t;

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

double riemann_serial(double lower_limit, double upper_limit, int number_of_rectangles, int thread_count){
	double rectangle_width =  (upper_limit-lower_limit)/number_of_rectangles;
	double combined_area = 0;
	#pragma omp parallel for reduction(+: combined_area)
	for(int i = 0; i < number_of_rectangles; i++)
	{
		combined_area += f(lower_limit+(i*rectangle_width));	
	}
	return combined_area*rectangle_width;
}

int main(int argument_counter, char * arguments[]){
	int lower_limit, upper_limit, number_of_rectangles, thread_count = 0;
	if(argument_counter >=4){
		lower_limit = (int)strtoul(arguments[1], NULL, 10);
		upper_limit = (int)strtoul(arguments[2], NULL, 10);
		number_of_rectangles = (int) strtoul(arguments[3], NULL, 10);
		thread_count = (int)strtoul(arguments[4], NULL, 10);
	}else{
		perror("Please input four integer arguments");
		exit(1);
	}
	walltime_t time;
	walltime_start(&time);
	printf("The result is = %f\n", riemann_serial(lower_limit,upper_limit,number_of_rectangles, thread_count));
	double elapsed = walltime_elapsed(&time);
	printf("Time taken = %f\n", elapsed);
	return 0;
}