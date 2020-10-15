#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <stdbool.h>
#include <pthread.h>

#ifndef PLAYERS_H_
#define PLAYERS_H_
#define _XOPEN_SOURCE 600

typedef struct timespec walltime_t;
walltime_t battle_time;

typedef struct
{
    int *validationArray;
    pthread_barrier_t barrier;
    pthread_mutex_t *player1_mutexes;
    pthread_mutex_t *player2_mutexes;
} shared_data_t;

typedef struct
{
    int player_num;
    int num; //battle order num
    int id;
    double hp;
    double energy;
    bool winner;
    bool loser;
    walltime_t time_lived;
    shared_data_t *shared_data;
    battlefield_data_t *battle_zone;
} pokemon_data_t;

typedef struct
{
    pokemon_data_t pk1;
    pokemon_data_t pk2;
}battlefield_data_t;

shared_data_t *shared_data;
battlefield_data_t *battle_zone;
pthread_t *p1_pokemon;
pthread_t *p2_pokemon;
pokemon_data_t *p1_pokemon_data_list;
pokemon_data_t *p2_pokemon_data_list;
#endif