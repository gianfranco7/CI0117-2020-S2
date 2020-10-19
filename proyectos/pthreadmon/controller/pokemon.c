#include "../model/pokemon.h"
#include "../controller/mapper.c"
#define AMOUNT_OF_POKEMON 3
#define ATTACK_BONUS 2.3
#define STARTING_POWER 0
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

void allocate_dynamic_memory()
{
	shared_data = (shared_data_t *)calloc(1, sizeof(shared_data_t));
	p0_pokemon = malloc((size_t)(AMOUNT_OF_POKEMON * sizeof(pthread_t)));
	p1_pokemon = malloc((size_t)(AMOUNT_OF_POKEMON * sizeof(pthread_t)));
	p0_pokemon_data_list = malloc((size_t)(AMOUNT_OF_POKEMON * sizeof(pokemon_data_t)));
	p1_pokemon_data_list = malloc((size_t)(AMOUNT_OF_POKEMON * sizeof(pokemon_data_t)));
}

int validate_pokemon(int scanned_id, int * validation_array)
{
	for (int i = 0; i < AMOUNT_OF_POKEMON; ++i)
	{
		if (validation_array[i] == scanned_id)
		{
			return 1;
		}
	}
	return 0;
}

void get_player_pokemon(pokemon_data_t * pokemon_data_list)
{
	int validation_array[AMOUNT_OF_POKEMON] = {0,0,0};
	int scanned_id = 0;
	for(int i = 0; i < AMOUNT_OF_POKEMON; ++i)
	{
		printf("Select pokémon %d: ", i);
		scanf("%d", &scanned_id);
		while(validate_pokemon(scanned_id, validation_array) == 1)
		{
			printf("Please select a different Pokémon species\n");
			printf("Select pokémon %d: ", i);
			scanf("%d", &scanned_id);
		}
		validation_array[i] = scanned_id;
		pokemon_data_list[i].id = scanned_id;
 	}
}

void get_pokemon()
{
	printf("PLAYER 1. Select 3 different pokémon species by typing each ID\n");
	get_player_pokemon(p0_pokemon_data_list);
	printf("Player 2. Select 3 different pokémon species by typing each ID\n");
	get_player_pokemon(p1_pokemon_data_list);
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
				printf(" name: %s\n", get_pokemon_species_name(p0_pokemon_data_list[j].id));
			}
			if (i == 1)
			{
				printf(" name: %s\n", get_pokemon_species_name(p1_pokemon_data_list[j].id));
			}
		}
	}
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

void initialize_mutexes(pokemon_data_t * pokemon_data_list)
{
	for(int i = 0; i < AMOUNT_OF_POKEMON; ++i)
	{
		pthread_mutex_init(&pokemon_data_list[i].my_mutex, NULL);
	}
}

void lock_mutexes(pokemon_data_t * pokemon_data_list)
{
	for(int i = 1; i < AMOUNT_OF_POKEMON; ++i)
	{
		pthread_mutex_lock(&pokemon_data_list[i].my_mutex);
	}
}

void assign_shared_data(shared_data_t * shared_data, pokemon_data_t * pokemon_data_list)
{
	pokemon_data_list->shared_data = shared_data;
}

void assign_active_pokemon_num(shared_data_t * shared_data, int player_num, int pokemon_num)
{
	if(player_num ==  0)
	{
		shared_data->active_p0_num = pokemon_num;
	}
	if(player_num == 1)
	{
		shared_data->active_p1_num = pokemon_num;
	}
}

void initialize_game()
{
	allocate_dynamic_memory();
	pthread_barrier_init(&shared_data->barrier, NULL, 6);
	initialize_mutexes(p0_pokemon_data_list);
	initialize_mutexes(p1_pokemon_data_list);
	initialize_data();
	get_pokemon();
	print_pokemon();
	assign_shared_data(shared_data, p0_pokemon_data_list);
	assign_shared_data(shared_data, p1_pokemon_data_list);
	start_battle_time(&battle_time);
	lock_mutexes(p0_pokemon_data_list);
	lock_mutexes(p1_pokemon_data_list);
	assign_active_pokemon_num(shared_data, 0, 0);
	assign_active_pokemon_num(shared_data, 0, 1);
}

double calculate_effectiveness(int target_pokemon_type, int attacking_move_type)
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
//duda con el bonus-siempre es 2.3?
double calculate_damage(double power, double effectiveness, double bonus)
{
	return floor(power * effectiveness * bonus) + 1;;
}

void *fight(void *args)
{
	pokemon_data_t *pokemon_data = (pokemon_data_t *)args;
	shared_data_t *shared_data = pokemon_data->shared_data;
	pthread_barrier_wait(&shared_data->barrier);

	pthread_mutex_lock(&pokemon_data->my_mutex);

	walltime_start(&pokemon_data->time_lived);

	while (pokemon_data->hp > 0)
	{

	}
	return NULL;
}

void create_threads()
{
	for (int i = 0; i < AMOUNT_OF_POKEMON; ++i)
	{
		p0_pokemon_data_list[i].shared_data = shared_data;
		p0_pokemon_data_list[i].hp = STARTING_HEALTH;
		p0_pokemon_data_list[i].power = STARTING_POWER;
		p0_pokemon_data_list[i].player_num = 0;
		p1_pokemon_data_list[i].shared_data = shared_data;
		p1_pokemon_data_list[i].hp = STARTING_HEALTH;
		p1_pokemon_data_list[i].power = STARTING_POWER;
		p1_pokemon_data_list[i].player_num = 1;
		pthread_create(&p0_pokemon[i], NULL, fight, (void *)&p0_pokemon_data_list[i]);
		pthread_create(&p1_pokemon[i], NULL, fight, (void *)&p1_pokemon_data_list[i]);
	}
}

void join_threads(pthread_t *threads, size_t thread_count)
{
	for (size_t i = 0; i < thread_count; ++i)
	{
		pthread_join(threads[i], NULL);
	}
}

void free_memory(pthread_t *p0threads, pthread_t *p1threads,
				 shared_data_t *shared_data, pokemon_data_t *p0_list, pokemon_data_t *p1_list)
{
	free(p0threads);
	free(p1threads);
	free(shared_data);
	free(p0_list);
	free(p1_list);
	for (int i = 0; i < AMOUNT_OF_POKEMON; ++i)
	{
		pthread_mutex_destroy(&p0_pokemon_data_list[i].my_mutex);
		pthread_mutex_destroy(&p1_pokemon_data_list[i].my_mutex);
	}
	//pthread_barrier_destroy(&shared_data->barrier);
}

void finish_game()
{
	join_threads(p0_pokemon, AMOUNT_OF_POKEMON);
	join_threads(p1_pokemon, AMOUNT_OF_POKEMON);
	free_memory(p0_pokemon, p1_pokemon, shared_data, p0_pokemon_data_list, p1_pokemon_data_list);
}