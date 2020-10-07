#include <stdio.h>
#include <stdlib.h>
#include "dna_sequences.h"


typedef struct{
	
	int first_sequence_occurrences[ALPHABET_SIZE];
	int second_sequence_occurrences[ALPHABET_SIZE];
	int common_letters_array[ALPHABET_SIZE];

}shared_data_t;

void get_first_sequence_occurrences(shared_data_t * shared_data)
{

	for(int i = 0; i < ALPHABET_SIZE; i++)
	{

		int occurrence_counter = 0;

		for(int j = 0; j < FIRST_SEQUENCE_SIZE; j++)
		{
			if(alphabet[i] == first_sequence[j])
			{
				
				occurrence_counter++;

			}
		}

		shared_data->first_sequence_occurrences[i] = occurrence_counter;
	}

}


void get_second_sequence_occurrences(shared_data_t * shared_data)
{

        for(int i = 0; i < ALPHABET_SIZE; i++)
        {

                int occurrence_counter = 0;

                for(int j = 0; j < SECOND_SEQUENCE_SIZE; j++)
                {
                        if(alphabet[i] == second_sequence[j])
                        {

                                occurrence_counter++;

                        }
                }

                shared_data->second_sequence_occurrences[i] = occurrence_counter;
        }

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

int main()
{
	
	shared_data_t * shared_data = (shared_data_t*)calloc(1, sizeof(shared_data_t));
	get_first_sequence_occurrences(shared_data);
	get_second_sequence_occurrences(shared_data);
	get_common_letters(shared_data);
	printf("Sequence 1. Occurrences:");
	print_occurrences(shared_data->first_sequence_occurrences);
	print_new_line();
	printf("Sequence 2. Occurrences:");
	print_occurrences(shared_data->second_sequence_occurrences);
	print_new_line();
	printf("Common letters:");
	print_common_letters(shared_data);
	print_new_line();

	return 0;
}
