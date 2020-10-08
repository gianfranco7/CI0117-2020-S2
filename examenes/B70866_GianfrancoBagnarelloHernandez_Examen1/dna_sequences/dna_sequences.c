#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <math.h>
#include "dna_sequences.h"


typedef struct
{
	
	int first_sequence_occurrences[ALPHABET_SIZE];
	int second_sequence_occurrences[ALPHABET_SIZE];
	int common_letters_array[ALPHABET_SIZE];
	pthread_mutex_t mutex;

}shared_data_t;


typedef struct
{

	int thread_num;
	int lower_limit;
	int upper_limit;
	int thread_calculated_occurrences[ALPHABET_SIZE];
	shared_data_t * shared_data;

}thread_data_t;


void * get_first_sequence_occurrences(void * args)
{

	thread_data_t* thread_data = (thread_data_t*) args;
	shared_data_t* shared_data = thread_data->shared_data;

	for(int i = 0; i < ALPHABET_SIZE; i++)
	{

		int occurrence_counter = 0;

		for(int j = thread_data->lower_limit; j < thread_data->upper_limit; j++)
		{

			if(alphabet[i] == first_sequence[j])
			{
				
				occurrence_counter++;
				//printf("Occurrence counter = %d\n", occurrence_counter);

			}

		}

		
		thread_data->thread_calculated_occurrences[i] = occurrence_counter;

		pthread_mutex_lock(&shared_data->mutex);

		//printf("Thread %d inserted these to main occurrence aray: ", thread_data->thread_num);
		//printf("%d ",shared_data->first_sequence_occurrences[i]);
		shared_data->first_sequence_occurrences[i]+=thread_data->thread_calculated_occurrences[i];
		//printf("\n");

		pthread_mutex_unlock(&shared_data->mutex);

	}


	return NULL;
}


void * get_second_sequence_occurrences(void * args)
{

	thread_data_t* thread_data = (thread_data_t*) args;
	shared_data_t* shared_data = thread_data->shared_data;

	for(int i = 0; i < ALPHABET_SIZE; i++)
	{

		int occurrence_counter = 0;

		for(int j = thread_data->lower_limit; j < thread_data->upper_limit; j++)
		{

			if(alphabet[i] == second_sequence[j])
			{
				
				occurrence_counter++;
				//printf("Occurrence counter = %d\n", occurrence_counter);

			}

		}

		
		thread_data->thread_calculated_occurrences[i] = occurrence_counter;

		pthread_mutex_lock(&shared_data->mutex);

		//printf("Thread %d inserted these to main occurrence aray: ", thread_data->thread_num);
		//printf("%d ",shared_data->second_sequence_occurrences[i]);
		shared_data->second_sequence_occurrences[i]+=thread_data->thread_calculated_occurrences[i];
		//printf("\n");

		pthread_mutex_unlock(&shared_data->mutex);

	}


	return NULL;
}


void print_occurrences(int * occurrence_array)
{

	for(int i = 0; i < ALPHABET_SIZE; i++)
        {
                if(occurrence_array[i] > 0)
                {
                
                        printf("%c=%d ", alphabet[i] ,occurrence_array[i]);

                }

        }

}


void get_common_letters(shared_data_t * shared_data)
{
	
	for(int i = 0; i < ALPHABET_SIZE; i++)
	{

		if(shared_data->first_sequence_occurrences[i] > 0 && shared_data->second_sequence_occurrences[i] > 0)
		{

			shared_data->common_letters_array[i] = 1;

		}
		

	}

}


void print_common_letters(shared_data_t * shared_data)
{
	
	for(int i = 0; i < ALPHABET_SIZE; i++)
	{
		
		if(shared_data->common_letters_array[i] == 1)
		{

			printf("%c ", alphabet[i]);
		
		}

	}

}


void print_new_line(){

	printf("\n");
	
}


void join_threads(pthread_t * threads, size_t thread_count)
{

        for (size_t i = 0; i < thread_count; ++i)
        {
                pthread_join(threads[i], NULL);
        }

}


void free_memory(pthread_t * threads, shared_data_t * shared_data, thread_data_t * sequence_one, thread_data_t * sequence_two)
{

	pthread_mutex_destroy(&shared_data->mutex);
    free(threads);
    free(shared_data);
    free(sequence_one);
    free(sequence_two);

}


int main(int argc, char * args[])
{

	size_t thread_count = 0;

    	if (argc >= 2) 
		{

        	thread_count = (size_t)strtoul(args[1], NULL, 10);

    	}
	else 
	{

        	printf("Error, invalid number of parameters\n");
        	return 1;

    	}

	shared_data_t * shared_data 	= (shared_data_t*)calloc(1, sizeof(shared_data_t));

	//SEQUENCE ONE BEGIN

	printf("ENTERED SEQUENCE ONE=---------------\n");

	pthread_t* threads1 		= malloc((size_t)(thread_count * sizeof(pthread_t)));
	thread_data_t* thread_data_list_sequence_one = malloc((size_t)(thread_count * sizeof(thread_data_t)));
	thread_data_list_sequence_one->shared_data = shared_data;
	pthread_mutex_init(&shared_data->mutex , NULL);


	double first_sequence_size = (double) FIRST_SEQUENCE_SIZE;	
	double sequence_one_range_length = first_sequence_size/thread_count;		
	printf("Length per thread calculated: %lf\n", sequence_one_range_length);

	//Case 1: More threads than spaces in array, create n threads where n equals the amount of spaces in the array
	if(sequence_one_range_length <= 1)
	{
		
		printf("Entered case 1: length per thread  <=  1\n");

		for (size_t i = 0; i  <  (size_t) FIRST_SEQUENCE_SIZE ; ++i) 
		{
				
			thread_data_list_sequence_one[i].shared_data = shared_data;
			thread_data_list_sequence_one[i].lower_limit = i;
			thread_data_list_sequence_one[i].upper_limit = i+1;
			thread_data_list_sequence_one[i].thread_num = i;
			printf("LOWER LIMIT SENT TO THREAD = %d\n", thread_data_list_sequence_one[i].lower_limit);
			printf("UPPER LIMIT SENT TO THREAD = %d\n", thread_data_list_sequence_one[i].upper_limit);
            pthread_create(&threads1[i], NULL, get_first_sequence_occurrences, (void*)&thread_data_list_sequence_one[i]);
			printf("Thread %zu created\n", i);

        	}	

	}
	

	//Case 2: More than one space per thread, create all threads available
	if(sequence_one_range_length > 1)
	{

		printf("Entered case 2: sequence_one_range_length > 1\n");
		//Case 2.1 If sequence length is a whole number, divide equally among threads
		if(floor(sequence_one_range_length) == sequence_one_range_length)
		{

			printf("Entered case 2.1: sequence_length is a whole number. Divide equally\n");
			
			int j = 0;
			for(size_t i = 0; i < thread_count; i++)
			{

				thread_data_list_sequence_one[i].shared_data = shared_data;
				thread_data_list_sequence_one[i].lower_limit = j;
				thread_data_list_sequence_one[i].upper_limit = j+sequence_one_range_length;
				j+=sequence_one_range_length;
				thread_data_list_sequence_one[i].thread_num = i;
                printf("LOWER LIMIT SENT TO THREAD = %d\n", thread_data_list_sequence_one[i].lower_limit);
				printf("UPPER LIMIT SENT TO THREAD = %d\n", thread_data_list_sequence_one[i].upper_limit);
                pthread_create(&threads1[i], NULL, get_first_sequence_occurrences, (void*)&thread_data_list_sequence_one[i]);
                printf("Thread %zu created\n", i);

			}

		}

		//Case 2.2 If sequence range length is not a whole number, divide length equally among all except one available threads, 
		//then load the last thread with remaining length
		else
		{

			printf("Entered case 2.2: non whole number sequence per thread\n");

			
			int j = 0;
			for(int i = 0; i < thread_count-1; ++i)
			{

			    thread_data_list_sequence_one[i].shared_data = shared_data;
                thread_data_list_sequence_one[i].lower_limit = j;
				thread_data_list_sequence_one[i].upper_limit = j+floor(sequence_one_range_length);
                thread_data_list_sequence_one[i].thread_num = i;
                printf("LOWER LIMIT SENT TO THREAD = %d\n", thread_data_list_sequence_one[i].lower_limit);
				printf("UPPER LIMIT SENT TO THREAD = %d\n", thread_data_list_sequence_one[i].upper_limit);
                pthread_create(&threads1[i], NULL, get_first_sequence_occurrences, (void*)&thread_data_list_sequence_one[i]);
                printf("Thread %d created\n", i);
				j+=floor(sequence_one_range_length);

			}


			//Load the last thread with any remaining rectangles
			
			thread_data_list_sequence_one[thread_count-1].shared_data = shared_data;

			thread_data_list_sequence_one[thread_count-1].lower_limit = j;

			thread_data_list_sequence_one[thread_count-1].upper_limit = FIRST_SEQUENCE_SIZE;

			thread_data_list_sequence_one[thread_count-1].thread_num = thread_count-1;

            printf("LOWER LIMIT SENT TO THREAD = %d\n", thread_data_list_sequence_one[thread_count-1].lower_limit);
			printf("UPPER LIMIT SENT TO THREAD = %d\n", thread_data_list_sequence_one[thread_count-1].upper_limit);

pthread_create(&threads1[thread_count-1], NULL, get_first_sequence_occurrences, (void *)&thread_data_list_sequence_one[thread_count-1]);


		}
		
	}

	join_threads(threads1, thread_count);

//SEQUENCE ONE END



//SECOND SEQUENCE START

printf("ENTERED SEQUENCE TWO=---------------\n");

	pthread_t* threads2 		= malloc((size_t)(thread_count * sizeof(pthread_t)));
	thread_data_t* thread_data_list_sequence_two = malloc((size_t)(thread_count * sizeof(thread_data_t)));
	thread_data_list_sequence_two->shared_data = shared_data;

	double second_sequence_size = (double) SECOND_SEQUENCE_SIZE;	
	double sequence_two_range_length = second_sequence_size/thread_count;		
	printf("Length per thread calculated: %lf\n", sequence_two_range_length);

	//Case 1: More threads than spaces in array, create n threads where n equals the amount of spaces in the array
	if(sequence_two_range_length <= 1)
	{
		
		printf("Entered case 1: length per thread  <=  1\n");

		for (size_t i = 0; i  <  (size_t) SECOND_SEQUENCE_SIZE ; ++i) 
		{
				
			thread_data_list_sequence_two[i].shared_data = shared_data;
			thread_data_list_sequence_two[i].lower_limit = i;
			thread_data_list_sequence_two[i].upper_limit = i+1;
			thread_data_list_sequence_two[i].thread_num = i;
			printf("LOWER LIMIT SENT TO THREAD = %d\n", thread_data_list_sequence_two[i].lower_limit);
			printf("UPPER LIMIT SENT TO THREAD = %d\n", thread_data_list_sequence_two[i].upper_limit);
            pthread_create(&threads2[i], NULL, get_second_sequence_occurrences, (void*)&thread_data_list_sequence_two[i]);
			printf("Thread %zu created\n", i);

        	}	

	}
	

	//Case 2: More than one space per thread, create all threads available
	if(sequence_two_range_length > 1)
	{

		printf("Entered case 2: sequence_one_range_length > 1\n");
		//Case 2.1 If sequence length is a whole number, divide equally among threads
		if(floor(sequence_two_range_length) == sequence_two_range_length)
		{

			printf("Entered case 2.1: sequence_length is a whole number. Divide equally\n");
			
			int j = 0;
			for(size_t i = 0; i < thread_count; i++)
			{

				thread_data_list_sequence_two[i].shared_data = shared_data;
				thread_data_list_sequence_two[i].lower_limit = j;
				thread_data_list_sequence_two[i].upper_limit = j+sequence_two_range_length;
				j+=sequence_two_range_length;
				thread_data_list_sequence_two[i].thread_num = i;
                printf("LOWER LIMIT SENT TO THREAD = %d\n", thread_data_list_sequence_two[i].lower_limit);
				printf("UPPER LIMIT SENT TO THREAD = %d\n", thread_data_list_sequence_two[i].upper_limit);
                pthread_create(&threads2[i], NULL, get_second_sequence_occurrences, (void*)&thread_data_list_sequence_two[i]);
                printf("Thread %zu created\n", i);

			}

		}

		//Case 2.2 If sequence range length is not a whole number, divide length equally among all except one available threads, 
		//then load the last thread with remaining length
		else
		{

			printf("Entered case 2.2: non whole number sequence per thread\n");

			
			int j = 0;
			for(int i = 0; i < thread_count-1; ++i)
			{

				thread_data_list_sequence_two[i].shared_data = shared_data;
                thread_data_list_sequence_two[i].lower_limit = j;
				thread_data_list_sequence_two[i].upper_limit = j+floor(sequence_two_range_length);
                thread_data_list_sequence_two[i].thread_num = i;
            	printf("LOWER LIMIT SENT TO THREAD = %d\n", thread_data_list_sequence_two[i].lower_limit);
				printf("UPPER LIMIT SENT TO THREAD = %d\n", thread_data_list_sequence_two[i].upper_limit);
                pthread_create(&threads2[i], NULL, get_second_sequence_occurrences, (void*)&thread_data_list_sequence_two[i]);
                printf("Thread %d created\n", i);
				j+=floor(sequence_two_range_length);

			}


			//Load the last thread with any remaining rectangles
			
			thread_data_list_sequence_two[thread_count-1].shared_data = shared_data;

			thread_data_list_sequence_two[thread_count-1].lower_limit = j;

			thread_data_list_sequence_two[thread_count-1].upper_limit = SECOND_SEQUENCE_SIZE;

			thread_data_list_sequence_two[thread_count-1].thread_num = thread_count-1;

            printf("LOWER LIMIT SENT TO THREAD = %d\n", thread_data_list_sequence_two[thread_count-1].lower_limit);
			printf("UPPER LIMIT SENT TO THREAD = %d\n", thread_data_list_sequence_two[thread_count-1].upper_limit);

pthread_create(&threads2[thread_count-1], NULL, get_second_sequence_occurrences, (void *)&thread_data_list_sequence_two[thread_count-1]);


		}
		
	}

	join_threads(threads2, thread_count);
//SECOND SEQUENCE END

	printf("Sequence 1. Occurrences:");	
	print_occurrences(shared_data->first_sequence_occurrences);
	print_new_line();
	printf("Sequence 2. Occurrences:");
	print_occurrences(shared_data->second_sequence_occurrences);
	print_new_line();
	get_common_letters(shared_data);
	printf("Common letters:");
	print_common_letters(shared_data);
	print_new_line();

	free_memory(threads1, shared_data, thread_data_list_sequence_one, thread_data_list_sequence_two);

	return 0;
}
