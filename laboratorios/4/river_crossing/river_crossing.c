#include <stdio.h>
#include <pthread.h>
#include <semaphore.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>
#include <unistd.h>

typedef struct{
    //totals
    size_t thread_count;
    size_t hacker_thread_count;
    size_t serf_thread_count;
    //current passengers
    size_t hackers;
    size_t serfs;
    //shared structures
    sem_t hackerQueue;
    sem_t serfQueue;
    pthread_mutex_t mutex;
    pthread_barrier_t barrier;
} shared_data_t;

typedef struct{
    int isCapitan;
    size_t thread_num;
    shared_data_t* shared_data;
} thread_data_t;

void random_sleep(useconds_t min_milliseconds, useconds_t max_milliseconds){
	useconds_t duration = min_milliseconds;
	useconds_t range = max_milliseconds - min_milliseconds;
	if ( range > 0 )
		duration += rand() % range;
	usleep( 1000 * duration );
}

void* hacker(void* args) {

    random_sleep(3000, 5000);

    thread_data_t* data = (thread_data_t*) args;

    size_t thread_num = data->thread_num;
    shared_data_t* shared_data = data->shared_data;

    pthread_mutex_lock(&shared_data->mutex);

    shared_data->hackers++;

    if (shared_data->hackers == 4) {
        for (size_t i = 0; i < 4; ++i) {
            sem_post(&shared_data->hackerQueue);
        }
        shared_data->hackers = 0;
        data->isCapitan = 1;

    } else if (shared_data->hackers == 2 && shared_data->serfs >= 2) {
        for (size_t i = 0; i < 2; ++i) {
            sem_post(&shared_data->hackerQueue);
            sem_post(&shared_data->serfQueue);
        }
        shared_data->hackers = 0;
        shared_data->serfs -= 2;
        data->isCapitan = 1;
    } else {
        pthread_mutex_unlock(&shared_data->mutex);
    }

    sem_wait(&shared_data->hackerQueue);

    //Board!
    printf("Thread %zu (Hacker): On board!\n", thread_num);

    //wait for 4 threads
    pthread_barrier_wait(&shared_data->barrier);

    if (data->isCapitan) {
        //Row boat!
        random_sleep(3000, 5000);
        printf("-------------------------------------------\n");
        printf("Thread %zu (Hacker): Row boat!!\n", thread_num);
        printf("===========================================\n");
        pthread_mutex_unlock(&shared_data->mutex);
    }

    return NULL;
}


void* serf(void* args){
    random_sleep(3000, 10000);

    thread_data_t* data = (thread_data_t*) args;

    size_t thread_num = data->thread_num;
    shared_data_t* shared_data = data->shared_data;

    pthread_mutex_lock(&shared_data->mutex);

    shared_data->serfs++;

    if (shared_data->serfs == 4) {
        for (size_t i = 0; i < 4; ++i) {
            sem_post(&shared_data->serfQueue);
        }
        shared_data->serfs = 0;
        data->isCapitan = 1;

    } else if (shared_data->serfs == 2 && shared_data->hackers >= 2) {
        for (size_t i = 0; i < 2; ++i) {
            sem_post(&shared_data->serfQueue);
            sem_post(&shared_data->hackerQueue);
        }
        shared_data->serfs = 0;
        shared_data->hackers -= 2;
        data->isCapitan = 1;
    } else {
        pthread_mutex_unlock(&shared_data->mutex);
    }

    sem_wait(&shared_data->serfQueue);

    //Board!
    printf("Thread %zu (Serf):On board!\n", thread_num);

    //wait for 4 threads
    pthread_barrier_wait(&shared_data->barrier);

    if (data->isCapitan) {
        //Row boat!
        random_sleep(3000, 5000);
        printf("-------------------------------------------\n");
        printf("Thread %zu (Serf): Row boat!!\n", thread_num);
        printf("===========================================\n");
        pthread_mutex_unlock(&shared_data->mutex);
    }

    return NULL;
}

int main(){

	//Variable declarations
	//thread_data_t thread_data;
	shared_data_t* shared_data = (shared_data_t*)calloc(1, sizeof(shared_data_t));
	srand(time(NULL));
	shared_data->thread_count = 4;//((rand()%10)+4)*4;	

	//Initialization of shared resources
	sem_init(&shared_data->hackerQueue, 0, 0);
	sem_init(&shared_data->serfQueue, 0, 0);
	pthread_mutex_init(&shared_data->mutex, NULL);
	pthread_barrier_init(&shared_data->barrier, NULL, 4);

	//Distribute thread types randomly
	shared_data->hacker_thread_count = 2;//rand()%shared_data->thread_count;
	shared_data->serf_thread_count = 2;//shared_data->thread_count - shared_data->hacker_thread_count;

	//Allocate memory for both types of threads 
	pthread_t* hacker_threads = malloc((size_t)(shared_data->hacker_thread_count * sizeof(pthread_t)));
	pthread_t* serf_threads = malloc((size_t)(shared_data->serf_thread_count * sizeof(pthread_t)));
	
	printf("Total number of hackers: %zu\n", shared_data->hacker_thread_count);
	printf("Total number of serfs: %zu\n", shared_data->serf_thread_count);

	thread_data_t* thread_data_list_serf = malloc((size_t)(2 * sizeof(thread_data_t)));
	thread_data_t* thread_data_list_hacker = malloc((size_t)(2* sizeof(thread_data_t)));

	for(size_t i = 0; i < 2; i++){
		thread_data_list_serf[i].isCapitan = 0;
		thread_data_list_serf[i].thread_num = i;
        	thread_data_list_serf[i].shared_data = shared_data;
		thread_data_list_hacker[i].isCapitan = 0;
                thread_data_list_hacker[i].thread_num = i;
                thread_data_list_hacker[i].shared_data = shared_data;
		pthread_create(&hacker_threads[i], NULL, hacker, (void*)&thread_data_list_hacker[i]);
		pthread_create(&serf_threads[i], NULL, serf, (void*)&thread_data_list_serf[i]);
	}

	for(size_t i = 0; i < 2; i++){
		pthread_join(serf_threads[i], NULL);
		pthread_join(hacker_threads[i], NULL);
	}
		
	//Memory cleanup
	free(hacker_threads);
	free(serf_threads);
	sem_destroy(&shared_data->hackerQueue);
	sem_destroy(&shared_data->serfQueue);
	pthread_mutex_destroy(&shared_data->mutex);
	return 0;
}
