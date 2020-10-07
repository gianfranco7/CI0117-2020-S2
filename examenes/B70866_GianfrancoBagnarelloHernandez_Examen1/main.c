#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>

typedef struct
{
	char occurrences[26];
}first_sequence_occurrences;

typedef struct
{
	char occurrences[26];
}second_sequence_occurrences;

typedef struct
{
	int common_letters[26];
}common_letters;


void * get_occurrences(void * args)
{
	//thread_data_t* thread_data = (thread_data_t*) args;

	

	return NULL;
}

void join_threads(pthread_t * threads, size_t thread_count)
{

        for (size_t i = 0; i < thread_count; ++i)
        {
                pthread_join(threads[i], NULL);
        }

}

int main(int argc, char *args[])
{

	size_t thread_count = 0;

    	if (argc >= 2) 
	{

        	thread_count = (size_t)strtoul(args[1], NULL, 10);
		thread_count*= 2;

    	}
	else 
	{

        	printf("Error, invalid number of parameters\n");
        	return 1;

    	}


	//Allocate memory
	pthread_t* threads = malloc((size_t)(thread_count * sizeof(pthread_t)));
	first_sequence_occurrences * occurrences1 = (first_sequence_occurrences*)calloc(1, sizeof(first_sequence_occurrences));
	second_sequence_occurrences * occurrences2 = (second_sequence_occurrences*)calloc(1, sizeof(second_sequence_occurrences));

	
	//Create threads for both sequences
	for(size_t i = 0; i < thread_count; i++){
		pthread_create(&threads[i], NULL, get_occurrences, (void*)&occurrences1);
		pthread_create(&threads[i], NULL, get_occurrences, (void*)&occurrences2);
	}


	//Memory cleanup
	join_threads(threads,thread_count);
	free(threads);

	return 0;
}
