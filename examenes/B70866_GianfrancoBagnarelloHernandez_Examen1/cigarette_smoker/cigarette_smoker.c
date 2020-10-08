#include <stdbool.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <errno.h>
#include <time.h>
#include <pthread.h>
#include <semaphore.h>

sem_t semaforos[3]; // vector de 3 semaforos uno por fumador
//sem_t agente;
pthread_cond_t cond_var;
pthread_mutex_t agente;
bool fumando = true;
typedef void *(*smokers_t)(void *);

 // El fumador cero tiene papel infinito.
void* fumador_0(void* arg)
{

	while (true)
	{
		if(fumando = true){
			pthread_cond_signal(&cond_var);
		}else{
			pthread_cond_wait(&cond_var, &agente);
		}
		nanosleep((struct timespec[]){{0, rand() % 3000000000}}, NULL); // Sleep para visualizar de mejor manera los threads
		printf("El fumador cero tiene papel y esta esperando para fumar.\n");
		sem_wait(&semaforos[0]); // Semáforo para esperar que el agente ponga los ingredientes que necesita el fumador
		printf("El fumador cero recibio tabaco y cerillos. Ahora esta fumando.\n");
		fumando = true;
		//sem_post(&agente);// Activa el semáforo del agente para que proceda a poner los siguientes ingredientes.
		pthread_mutex_unlock(&agente);
	}

	return NULL;
}
 
//El fumador uno tiene cerillos infinitos.
void* fumador_1(void* arg)
{

	while (true)
	{
		if(fumando = true){
			pthread_cond_signal(&cond_var);
		}else{
			pthread_cond_wait(&cond_var, &agente);
		}
		nanosleep((struct timespec[]){{0, rand() % 30000000000}}, NULL);
		printf("El fumador uno tiene cerillos y esta esperando para fumar.\n");
		sem_wait(&semaforos[1]);
		printf("El fumador uno recibio tabaco y papel. Ahora esta fumando.\n");
		fumando = true;
		//sem_post(&agente);
		pthread_mutex_unlock(&agente);
	}
	return NULL;
}

//El fumador dos tiene tacabo infinito.
void* fumador_2(void* arg)
{
	while (true)
	{
		if(fumando = true){
			pthread_cond_signal(&cond_var);
		}else{
			pthread_cond_wait(&cond_var, &agente);
		}
		nanosleep((struct timespec[]){{0, rand() % 3000000000}}, NULL);
		printf("El fumador dos tiene tabaco y esta esperando para fumar.\n");

		sem_wait(&semaforos[2]);
		printf("El fumador dos recibio cerillos y papel. Ahora esta fumando.\n");
		fumando = true;
	    //sem_post(&agente);
		pthread_mutex_unlock(&agente);
	}
	return NULL;
}

int main(int argc, char* arvg[])
{
	srand(time(NULL));
	//sem_init(&agente, 0, 1); // semaforo del agente.
	pthread_mutex_init(&agente, NULL);
	pthread_cond_init(&cond_var, NULL);

	pthread_t fumadores[3];// vector de threads
	smokers_t metodos_fumadores[3] = { fumador_0, fumador_1, fumador_2 };// Metodos para los threads.

	for (int i = 0; i < 3; ++i) // Aqui se inicializa el semaforo de cada fumador *thread* y se crea cada thread con su respectiva funcion.
	{
		sem_init(&semaforos[i], 0, 0);
		if(pthread_create(&fumadores[i], NULL, metodos_fumadores[i], NULL) == EAGAIN)
		{
			perror("Recursos insuficientes, no se han creado los hilos\n");
			return 0;
		}
	}


	while (true) // aca el agente va a empeza a distribuir 2 ingredientes en cada iteracion, solo un thread podra tomarlos para evitar deadlocks.
	{
		//sem_wait(&agente);// espera para que que el fumador que este fumando termine de fumar y el pueda poner  los materiales de nuevo.
		fumando = false;
		pthread_mutex_lock(&agente);
		sem_post(&semaforos[rand() % 3]); // distribuye dos materiales para un solo fumador.
	}

	for (int i = 0; i < 3; ++i)
	{
		pthread_join(fumadores[i], NULL);
	}

}