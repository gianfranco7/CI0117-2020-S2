#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <semaphore.h>

/*
Modifique su solución al ejercicio hello_ordered_mutex para que en lugar de usar exclusión mutua (mutex), use un arreglo de semáforos, uno porcada thread. Corra su solución y mida el tiempo de ejecución. ¿Permite este mecanismo de control de concurrencia resolver este problema de manera predecible? Explique las diferencias entre usar exclusión mutua y semáforos para resolver este problema en un archivo rationale.md dentrode su carpeta hello_ordered_semaphore.
*/

struct timespec start, finish;                   

typedef struct {
    size_t message;
    size_t chose_thread;
    size_t next_thread;
} shared_message_t;

typedef struct {
    size_t thread_num;
    shared_message_t *shared_message;
} thread_data_t;

//semaphore array declaration
sem_t *semaphores;

void* helloWorld(void* args) {
    
    thread_data_t* data = (thread_data_t*)args;
    size_t thread_num = data->thread_num;
    shared_message_t* shared_message = data->shared_message;
    
    //while (thread_num != shared_message->next_thread);	
    
    if (thread_num == shared_message->chose_thread) {
        shared_message->message = 2021;
    }
    
    printf("Hello world from thread number # %zu. The message is: %zu\n", thread_num, shared_message->message);
    ++shared_message->next_thread;
    //unlock semaphore of current thread
    sem_post(&semaphores[thread_num]);
    //unlock semaphore of next thread
    sem_post(&semaphores[shared_message->next_thread]);

    return NULL;
}


int main(int argc, char* arg[]) {
    clock_gettime(CLOCK_REALTIME, &start);
    size_t thread_count = 0;
    size_t chosen_thread = 0;

    if (argc >= 3) {
        thread_count = (size_t)strtoul(arg[1], NULL, 10);
        chosen_thread = (size_t)strtoul(arg[2], NULL, 10);

        if (chosen_thread < 0 || chosen_thread >= thread_count) {
            fprintf(stderr, "Error, invalid parameters\n");
            return 1;
        }
    } else {
        fprintf(stderr, "Error, invalid number of parameters\n");
        return 1;
    }

    pthread_t* threads = malloc((size_t)(thread_count * sizeof(pthread_t)));
    //dynamic memory allocation for semaphore array
    semaphores = malloc((thread_count * sizeof(sem_t)));
    shared_message_t* shared_message = (shared_message_t*)calloc(1, sizeof(shared_message_t));

    shared_message->message = 2020;
    shared_message->chose_thread = chosen_thread;
    shared_message->next_thread = 0;

    thread_data_t* thread_data_list = malloc((size_t)(thread_count * sizeof(thread_data_t)));

    //semaphore initialization, first semaphore starts with value 1, the rest with 0 (disabled)
    for(size_t i = 0; i < thread_count; ++i){
	if(i == 0){
		sem_init(&semaphores[i], 0, 1);
	}else{
    		sem_init(&semaphores[i], 0, 0);
	}
    }
	 
    //thread creation 
    for (size_t i = 0; i < thread_count; ++i) {
	//lock the semaphore of current thread
	sem_wait(&semaphores[i]);
        thread_data_list[i].thread_num = i;
        thread_data_list[i].shared_message = shared_message;
        pthread_create(&threads[i], NULL, helloWorld, (void*)&thread_data_list[i]);
    }

    //thread joining
    for (size_t i = 0; i < thread_count; ++i) {
        pthread_join(threads[i], NULL);
    }

    printf("Hello world from main thread\n");

    //dynamic memory release
    free(threads);
    free(shared_message);
    free(thread_data_list);
    free(semaphores);
    for(size_t i = 0; i< thread_count; ++i){
    	sem_destroy(&semaphores[i]);
    }
    clock_gettime(CLOCK_REALTIME, &finish);
    unsigned int time_taken = finish.tv_nsec - start.tv_nsec;
    double total_time = (double) time_taken/1000000000;
    printf("Time taken: %fs\n", total_time);
    return 0;
}
