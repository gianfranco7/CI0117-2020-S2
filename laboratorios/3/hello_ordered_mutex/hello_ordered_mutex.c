#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

/*
Modifique la solución del ejemplo de Hello World con espera activa visto en clase para obligar a los hilos de ejecución a saludar en orden en la salida estándar. Use una colección de mutexes, uno por cada thread. Haga que únicamente el thread 0 tenga su mutex habilitado. Cuando el thread 0 ha saludado, habilita el mutex del siguiente thread y así sucesivamente. Corra su solución y mida el tiempo de ejecución. ¿Permite este mecanismo de control de concurrencia resolver este problema de manera predecible?
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

//mutex array declaration
pthread_mutex_t *locks;

void* helloWorld(void* args) {
    
    thread_data_t* data = (thread_data_t*)args;
    size_t thread_num = data->thread_num;
    shared_message_t* shared_message = data->shared_message;
    
    //while (thread_num != shared_message->next_thread);	
    
    if (thread_num == shared_message->chose_thread) {
        shared_message->message = 2021;
    }
    
    printf("Hello world from thread number # %zu. The message is: %zu\n", thread_num, shared_message->message);
    pthread_mutex_unlock(&locks[shared_message->next_thread]);
    ++shared_message->next_thread;
    //unlock mutex of current thread
    pthread_mutex_unlock(&locks[thread_num]);
    //unlock mutex of next thread
    pthread_mutex_unlock(&locks[shared_message->next_thread]);
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
    //dynamic memory allocation for mutex array
    locks = malloc((thread_count * sizeof(pthread_mutex_t)));
    shared_message_t* shared_message = (shared_message_t*)calloc(1, sizeof(shared_message_t));

    shared_message->message = 2020;
    shared_message->chose_thread = chosen_thread;
    shared_message->next_thread = 0;

    thread_data_t* thread_data_list = malloc((size_t)(thread_count * sizeof(thread_data_t)));

    //mutex initialization
    for(size_t i = 0; i < thread_count; ++i){
    	pthread_mutex_init(&locks[i], NULL);
    }
	
    
    //lock every thread except first one
    for(size_t i = 1; i < thread_count; i++){
    	pthread_mutex_lock(&locks[i]);
    }

    //thread creation 
    for (size_t i = 0; i < thread_count; ++i) {
	//lock the mutex of current thread
	    pthread_mutex_lock(&locks[i]);
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
    free(locks);
    for(size_t i = 0; i < thread_count; i++){
    	pthread_mutex_destroy(&locks[i]);
    }
    free(thread_data_list);
    clock_gettime(CLOCK_REALTIME, &finish);

    unsigned int time_taken = finish.tv_nsec - start.tv_nsec;
    double total_time = (double) time_taken/1000000000;
    printf("Time taken: %fs\n", total_time);
    return 0;
}
