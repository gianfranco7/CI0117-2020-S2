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

int validate_pokemon(int scanned_id, int *validation_array)
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

void get_player_pokemon(pokemon_data_t *pokemon_data_list)
{
	signed int validation_array[AMOUNT_OF_POKEMON] = {-1, -1, -1};
	int scanned_id = 0;
	for (int i = 0; i < AMOUNT_OF_POKEMON; ++i)
	{
		printf("Select pokémon %d: ", i);
		scanf("%d", &scanned_id);
		while (validate_pokemon(scanned_id, validation_array) == 1)
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
	return walltime_elapsed(&battle_time);
}

void initialize_mutexes(pokemon_data_t *pokemon_data_list)
{
	for (int i = 0; i < AMOUNT_OF_POKEMON; ++i)
	{
		pthread_mutex_init(&pokemon_data_list[i].my_mutex, NULL);

	}
	pthread_mutex_init(&shared_data->charged_attack_mutex, NULL);
	pthread_mutex_init(&shared_data->p0mutex, NULL);
	pthread_mutex_init(&shared_data->p1mutex, NULL);
}

void lock_mutexes(pokemon_data_t *pokemon_data_list)
{
	for (int i = 1; i < AMOUNT_OF_POKEMON; ++i)
	{
		pthread_mutex_lock(&pokemon_data_list[i].my_mutex);
	}
}

void assign_shared_data(shared_data_t *shared_data, pokemon_data_t *pokemon_data_list)
{
	pokemon_data_list->shared_data = shared_data;
}

void assign_active_pokemon_num(shared_data_t *shared_data, int player_num, int pokemon_num)
{
	if (player_num == 0)
	{
		shared_data->active_p0_num = pokemon_num;
	}
	if (player_num == 1)
	{
		shared_data->active_p1_num = pokemon_num;
	}
}

void initialize_synchronization_structures()
{
	pthread_barrier_init(&shared_data->barrier0, NULL, 3);
	pthread_barrier_init(&shared_data->barrier1, NULL, 3);
	initialize_mutexes(p0_pokemon_data_list);
	initialize_mutexes(p1_pokemon_data_list);
}

void initialize_game()
{
	allocate_dynamic_memory();
	initialize_synchronization_structures();
	initialize_data();
	get_pokemon();
	print_pokemon();
	assign_shared_data(shared_data, p0_pokemon_data_list);
	assign_shared_data(shared_data, p1_pokemon_data_list);
	start_battle_time(&battle_time);
	//lock_mutexes(p0_pokemon_data_list);
	//lock_mutexes(p1_pokemon_data_list);
	assign_active_pokemon_num(shared_data, 0, 0);
	assign_active_pokemon_num(shared_data, 0, 0);
}

double calculate_effectiveness(int target_pokemon_type, int attacking_move_type)
{
	if (weaknesses_matrix[target_pokemon_type][attacking_move_type])
	{
		//printf("It'super effective!!!\n");
		return 1.6;
	}
	else if (resistances_matrix[target_pokemon_type][attacking_move_type])
	{
		//printf("It's mediumly effective!!!");
		return 0.625;
	}
	else if (immunities_matrix[target_pokemon_type][attacking_move_type])
	{
		//printf("It's not very effective...\n");
		return 0.390625;
	}
	else
	{
		return 1;
	}
}

double calculate_damage(double power, double effectiveness, double bonus)
{
	return floor(power * effectiveness * bonus) + 1;
}

double get_charged_attack_energy(int pokemon_id)
{
	return get_move_energy(get_pokemon_charged_move_id(pokemon_id));
}

attack_data_t do_charged_attack(int my_id, int opponent_id, pokemon_data_t my_data_list, pokemon_data_t opponent_data_list)
{
	pthread_mutex_lock(&shared_data->charged_attack_mutex);
	attack_data_t attack_data;
	//printf("My energy parameter recieved by charged attack function = %lf\n", my_data_list.power);
	//printf("Opponent health parameter recieved by charged attack function = %lf\n", opponent_data_list.hp);
	//printf("Pokemon ID = %d entered charged attack function\n", my_id);
	double energy = get_move_energy(get_pokemon_charged_move_id(my_data_list.id));
	double effectiveness = calculate_effectiveness(get_pokemon_type_id(opponent_id), get_pokemon_type_id(my_id));
	//printf("Effectiveness calculated = %lf\n", effectiveness);
	double damage = calculate_damage(energy, effectiveness, ATTACK_BONUS);
	//printf("Damage calculated = %lf\n", damage);
	//printf("Opponent health before charged hit = %lf\n", opponent_data_list.hp);
	opponent_data_list.hp -= damage;
	attack_data.opponent_hp = (opponent_data_list.hp -= damage);
	//printf("Opponent health after charged hit = %lf\n", opponent_data_list.hp);
	//printf("My energy before charged hit = %lf\n", my_data_list.power);
	my_data_list.power -= energy;
	attack_data.my_energy = (my_data_list.power -= energy);
	//printf("My energy after charged hit = %lf\n", my_data_list.power);
	//printf("Pokemon will usleep for %d microseconds\n", get_move_cooldown(get_pokemon_charged_move_id(my_id)));
	usleep(get_move_cooldown(get_pokemon_charged_move_id(my_id)));
	pthread_mutex_unlock(&shared_data->charged_attack_mutex);
	return attack_data;
}

attack_data_t do_fast_attack(int my_id, int opponent_id, pokemon_data_t my_data_list, pokemon_data_t opponent_data_list)
{
	attack_data_t attack_data;
	//printf("My energy parameter recieved by fast attack function = %lf\n", my_data_list.power);
	//printf("Opponent health parameter recieved by fast attack function = %lf\n", opponent_data_list.hp);
	//printf("The energy cost of my fast attack is %d\n", get_move_energy(get_pokemon_charged_move_id(my_id)));
	//printf("Pokemon ID = %d entered fast attack function\n", my_id);
	double energy = get_move_energy_gain(get_pokemon_fast_move_id(my_data_list.id));
	double effectiveness = calculate_effectiveness(get_pokemon_type_id(opponent_id), get_pokemon_type_id(my_id));
	//printf("Effectiveness calculated = %lf\n", effectiveness);
	double damage = calculate_damage(energy, effectiveness, ATTACK_BONUS);
	//printf("Damage calculated = %lf\n", damage);
	//printf("Opponent health before fast hit = %lf\n", opponent_data_list.hp);
	opponent_data_list.hp -= damage;
	attack_data.opponent_hp = (opponent_data_list.hp -= damage);
	//printf("Opponent health after fast hit = %lf\n", opponent_data_list.hp);
	//printf("My energy before fast hit = %lf\n", my_data_list.power);
	my_data_list.power += energy;
	attack_data.my_energy = (my_data_list.power += energy);
	//printf("My energy after fast hit = %lf\n", my_data_list.power);
	//printf("Pokemon will usleep for %d microseconds\n", get_move_cooldown(get_pokemon_charged_move_id(my_id)));
	usleep(get_move_cooldown(get_pokemon_fast_move_id(my_id)));
	return attack_data;
}

void *fight0(void *args)
{
	pokemon_data_t *pokemon_data = (pokemon_data_t *)args;
	//pthread_barrier_wait(&shared_data->barrier0);
	//pthread_mutex_lock(&pokemon_data->my_mutex);
	pthread_mutex_lock(&shared_data->p0mutex);
	int my_num = shared_data->active_p0_num;
	int opponent_num = shared_data->active_p1_num;
	int my_id = p0_pokemon_data_list[my_num].id;
	int opponent_id = p1_pokemon_data_list[opponent_num].id;
	walltime_start(&p0_pokemon_data_list[my_num].time_lived);
	while (p0_pokemon_data_list[my_num].hp > 0 && p1_pokemon_data_list[opponent_num].hp > 0)
	{
		if (p0_pokemon_data_list[my_num].power >= get_move_energy(get_pokemon_charged_move_id(p0_pokemon_data_list[my_num].id)))
		{
			attack_data_t attack_data;
			attack_data = do_charged_attack(my_id, opponent_id, p0_pokemon_data_list[my_num], p1_pokemon_data_list[opponent_num]);
			p0_pokemon_data_list[my_num].power = attack_data.my_energy;
			p1_pokemon_data_list[opponent_num].hp = attack_data.opponent_hp;
		}
		else
		{
			attack_data_t attack_data;
			attack_data = do_fast_attack(my_id, opponent_id, p0_pokemon_data_list[my_num], p1_pokemon_data_list[opponent_num]);
			p0_pokemon_data_list[my_num].power = attack_data.my_energy;
			p1_pokemon_data_list[opponent_num].hp = attack_data.opponent_hp;
		}
	}

	//if i won
	if(p0_pokemon_data_list[my_num].hp > 0 && p1_pokemon_data_list[opponent_num].hp <= 0)
	{
		walltime_elapsed(&p1_pokemon_data_list[opponent_num].time_lived);
		if(opponent_num != 2)
		{
			//printf("(F0 I WON) Pokemon ID = %d won\n", my_id); 
			printf("%s won\n",get_pokemon_species_name(my_id));
			printf("%s lost\n", get_pokemon_species_name(opponent_id));
			//shared_data->active_p1_num++;
		}
	}
	//if i lost
	if(p0_pokemon_data_list[my_num].hp <= 0 && p1_pokemon_data_list[opponent_num].hp > 0)
	{
		walltime_elapsed(&p0_pokemon_data_list[my_num].time_lived);
		if (my_num != 2)
		{
			printf("%s won\n",get_pokemon_species_name(opponent_id));
			printf("%s lost\n", get_pokemon_species_name(my_id));
			//shared_data->active_p0_num++;
		}
	}
	//pthread_mutex_unlock(&pokemon_data->my_mutex);
	pthread_mutex_unlock(&shared_data->p0mutex);
	return NULL;
}

void *fight1(void *args)
{
	pokemon_data_t *pokemon_data = (pokemon_data_t *)args;
	//pthread_barrier_wait(&shared_data->barrier1);
	//pthread_mutex_lock(&pokemon_data->my_mutex);
	pthread_mutex_lock(&shared_data->p1mutex);
	int my_num = shared_data->active_p1_num;
	int opponent_num = shared_data->active_p0_num;
	int my_id = p1_pokemon_data_list[my_num].id;
	int opponent_id = p0_pokemon_data_list[opponent_num].id;
	walltime_start(&p1_pokemon_data_list[my_num].time_lived);
	while (p1_pokemon_data_list[my_num].hp > 0 && p0_pokemon_data_list[opponent_num].hp > 0)
	{
		if (p1_pokemon_data_list[my_num].power >= get_move_energy(get_pokemon_charged_move_id(p1_pokemon_data_list[my_num].id)))
		{
			attack_data_t attack_data;
			attack_data = do_charged_attack(my_id, opponent_id, p1_pokemon_data_list[my_num], p0_pokemon_data_list[opponent_num]);
			p1_pokemon_data_list[my_num].power = attack_data.my_energy;
			p0_pokemon_data_list[opponent_num].hp = attack_data.opponent_hp;
		}
		else
		{
			attack_data_t attack_data;
			attack_data = do_fast_attack(my_id, opponent_id, p1_pokemon_data_list[my_num], p0_pokemon_data_list[opponent_num]);
			p1_pokemon_data_list[my_num].power = attack_data.my_energy;
			p0_pokemon_data_list[opponent_num].hp = attack_data.opponent_hp;
		}
	}

	//if i won
	if(p1_pokemon_data_list[my_num].hp > 0 && p0_pokemon_data_list[opponent_num].hp <= 0)
	{
		walltime_elapsed(&p0_pokemon_data_list[opponent_num].time_lived);
		if(opponent_num != 2)
		{
			printf("%s won\n",get_pokemon_species_name(my_id));
			printf("%s lost\n", get_pokemon_species_name(opponent_id));
			shared_data->active_p0_num++;
		}
	}
	//if i lost
	if(p1_pokemon_data_list[my_num].hp <= 0 && p0_pokemon_data_list[opponent_num].hp > 0)
	{
		walltime_elapsed(&p1_pokemon_data_list[my_num].time_lived);
		if (my_num != 2)
		{
			printf("%s won\n",get_pokemon_species_name(opponent_id));
			printf("%s lost\n", get_pokemon_species_name(my_id));
			shared_data->active_p1_num++;
		}
	}
	//pthread_mutex_unlock(&pokemon_data->my_mutex);
	pthread_mutex_unlock(&shared_data->p1mutex);
	return NULL;
}

void load_data_list(pokemon_data_t pokemon_data_list, int player_num)
{
	pokemon_data_list.shared_data = shared_data;
	pokemon_data_list.hp = STARTING_HEALTH;
	pokemon_data_list.power = STARTING_POWER;
	pokemon_data_list.player_num = player_num;
}

void create_thread(pthread_t pokemon, pokemon_data_t pokemon_data_list, int function_num)
{
	pthread_create(&pokemon, NULL, fight0, (void *)&pokemon_data_list);
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
		pthread_create(&p0_pokemon[i], NULL, fight0, (void *)&p0_pokemon_data_list[i]);
		pthread_create(&p1_pokemon[i], NULL, fight1, (void *)&p1_pokemon_data_list[i]);
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
	pthread_mutex_destroy(&shared_data->charged_attack_mutex);
	pthread_barrier_destroy(&shared_data->barrier0);
	pthread_barrier_destroy(&shared_data->barrier1);
}

void finish_game()
{
	join_threads(p0_pokemon, AMOUNT_OF_POKEMON);
	join_threads(p1_pokemon, AMOUNT_OF_POKEMON);
	free_memory(p0_pokemon, p1_pokemon, shared_data, p0_pokemon_data_list, p1_pokemon_data_list);
}