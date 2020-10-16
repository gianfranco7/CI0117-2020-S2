#include "../model/pokemon.h"
#include "../controller/mapper.c"
#define AMOUNT_OF_POKEMON 3
#define ATTACK_BONUS 2.3
#define STARTING_ENERGY 0
#define STARTING_HEALTH 1500

void walltime_start(walltime_t *start)
{
	clock_gettime(CLOCK_MONOTONIC, start);
}

double walltime_elapsed(const walltime_t *start)
{
	walltime_t finish;
	clock_gettime(CLOCK_MONOTONIC, &finish);
	double elapsed = (finish.tv_sec - start->tv_sec);
	elapsed += (finish.tv_nsec - start->tv_nsec) / 1000000000.0;
	return elapsed;
}

bool validate_pokemon(int *array, int pokemon_id)
{
	for (int i = 0; i < AMOUNT_OF_POKEMON; ++i)
	{
		if (array[i] == pokemon_id)
		{
			return false;
		}
	}
	return true;
}

void get_pokemon()
{
	for (int i = 0; i < 2; ++i)
	{
		printf("Please PLAYER %d enter POKEMON\n", i);
		for (int j = 0; j < AMOUNT_OF_POKEMON; ++j)
		{
			printf("%d: ", j);
			if (i == 0)
			{
				scanf("%d", &p1_pokemon_data_list[j].id);
			}
			if (i == 1)
			{
				scanf("%d", &p2_pokemon_data_list[j].id);
			}
		}
	}
}

void print_pokemon()
{
	for (int i = 0; i < 2; ++i)
	{
		printf("PLAYER %d POKEMON\n", i);
		for (int j = 0; j < AMOUNT_OF_POKEMON; ++j)
		{
			printf("%d ", j);
			if (i == 0)
			{
				printf(" name: %s\n", get_pokemon_species_name(p1_pokemon_data_list[j].id));
			}
			if (i == 1)
			{
				printf(" name: %s\n", get_pokemon_species_name(p2_pokemon_data_list[j].id));
			}
		}
	}
	printf("PRINT POKEMON CHECKPOINT\n");
}

void start_battle_time()
{
	walltime_start(&battle_time);
}

double get_battle_time()
{
	double elapsed = walltime_elapsed(&battle_time);
	return elapsed;
}

void begin_battle()
{
	printf("------------BEGIN BATTLE!!!------------\n");
	start_battle_time();
}

double calculate_damage(int target_pokemon_type, int attacking_move_type)
{
	if (weaknesses_matrix[target_pokemon_type][attacking_move_type])
	{
		printf("It'super effective!!!\n");
		return 1.6;
	}
	else if (resistances_matrix[target_pokemon_type][attacking_move_type])
	{
		return 0.625;
	}
	else if (immunities_matrix[target_pokemon_type][attacking_move_type])
	{
		printf("It's not very effective...\n");
		return 0.390625;
	}
	else
	{
		return 1;
	}
}

void *fight(void *args)
{
	pokemon_data_t *pokemon_data = (pokemon_data_t *)args;
	shared_data_t *shared_data = pokemon_data->shared_data;
	pthread_barrier_wait(&shared_data->barrier);

	if (pokemon_data->player_num == 0)
	{
		pthread_mutex_lock(&shared_data->player1_mutexes[pokemon_data->num]);
	}
	if (pokemon_data->player_num == 1)
	{
		pthread_mutex_lock(&shared_data->player2_mutexes[pokemon_data->num]);
	}

	pthread_mutex_lock(&pokemon_data->my_mutex);

	while (pokemon_data->hp > 0)
	{
		if (pokemon_data->energy > get_move_energy(get_move_type_id(pokemon_data->id))) 
		{
			double damage = calculate_damage(get_pokemon_type_id(battle_zone->pk1.id), 
			get_pokemon_type_id(battle_zone->pk2.id));
		}
		else
		{
			
		}
	}

	return NULL;
}

void join_threads(pthread_t *threads, size_t thread_count)
{
	for (size_t i = 0; i < thread_count; ++i)
	{
		pthread_join(threads[i], NULL);
	}
}

void free_memory(pthread_t *p1threads, pthread_t *p2threads,
				 shared_data_t *shared_data, pokemon_data_t *p1, pokemon_data_t *p2)
{
	free(p1threads);
	free(p2threads);
	free(shared_data);
	free(p1);
	free(p2);
	for (int i = 0; i < AMOUNT_OF_POKEMON; ++i)
	{
		pthread_mutex_destroy(&shared_data->player1_mutexes[i]);
		pthread_mutex_destroy(&shared_data->player2_mutexes[i]);
	}
	//pthread_barrier_destroy(&shared_data->barrier);
}