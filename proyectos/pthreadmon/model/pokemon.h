#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <pthread.h>
#include <math.h>
#include <unistd.h>
#ifndef PLAYERS_H_
#define PLAYERS_H_
#define _XOPEN_SOURCE 600

typedef struct timespec walltime_t;
walltime_t battle_time;

typedef struct
{
    pthread_barrier_t barrier0;
    pthread_barrier_t barrier1;
    pthread_mutex_t charged_attack_mutex;
    pthread_mutex_t p0mutex;
    pthread_mutex_t p1mutex;
    int active_p0_num;  //battle order num
    int active_p1_num;  //battle order_num
} shared_data_t;

typedef struct
{
    int player_num;
    int num; //battle order num
    int id;
    double hp;
    double power;
    walltime_t time_lived;
    shared_data_t *shared_data;
} pokemon_data_t;

typedef struct
{
    double my_energy;
    double opponent_hp;
} attack_data_t;

shared_data_t *shared_data;
pthread_t *p0_pokemon;
pthread_t *p1_pokemon;
pokemon_data_t *p0_pokemon_data_list;
pokemon_data_t *p1_pokemon_data_list;
#endif