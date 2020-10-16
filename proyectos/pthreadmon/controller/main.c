#include "pokemon.c"
int main()
{
	shared_data = (shared_data_t *)calloc(1, sizeof(shared_data_t));
	battle_zone = (battlefield_data_t *)calloc(1, sizeof(battlefield_data_t));
	p1_pokemon = malloc((size_t)(AMOUNT_OF_POKEMON * sizeof(pthread_t)));
	p2_pokemon = malloc((size_t)(AMOUNT_OF_POKEMON * sizeof(pthread_t)));
	p1_pokemon_data_list = malloc((size_t)(AMOUNT_OF_POKEMON * sizeof(pokemon_data_t)));
	p2_pokemon_data_list = malloc((size_t)(AMOUNT_OF_POKEMON * sizeof(pokemon_data_t)));

	shared_data->player1_mutexes = malloc((AMOUNT_OF_POKEMON * sizeof(pthread_mutex_t)));
	shared_data->player2_mutexes = malloc((AMOUNT_OF_POKEMON * sizeof(pthread_mutex_t)));
	pthread_barrier_init(&shared_data->barrier, NULL, 6);

	for (int i = 0; i < AMOUNT_OF_POKEMON; ++i)
	{
		pthread_mutex_init(&shared_data->player1_mutexes[i], NULL);
		pthread_mutex_init(&shared_data->player2_mutexes[i], NULL);
	}
	//preguntar si es mejor arreglos de mutexes separados o que cada pokemon tenga su propio mutex
	for(int i = 1; i < AMOUNT_OF_POKEMON; ++i)
	{
		pthread_mutex_init(&p1_pokemon_data_list[i].my_mutex, NULL);
		pthread_mutex_init(&p2_pokemon_data_list[i].my_mutex, NULL);
	}

	//SELECTION SCREEN
	//Create 7 different buttons, 3 for each user, and one to start the battle.
	//Each user button will be a selection screen to choose from the pokemon list in mapper.h
	//The order of the buttons, first to last (downwards) will be the pokemon battle order
	//Every user selected pokemon must be of a different species
	initialize_data();
	get_pokemon();
	printf("CHECKPOINT1\n");
	print_pokemon();
	printf("CHECKPOINT2\n");
	//BATTLE SCREEN
	//>>Initial time measurement
	// 1st button pokemon of userleft vs 1st button pokemon of userright
	//>>Secondary time measurement: each pokemon has a time attribute that registers the amount of time it lasted while HP>0
	// Fight until HP reaches 0 in either
	// When HP 0 on either, replace pokemon with second on the list, then the third
	// Winner is the trainer who still has pokemons with HP > 0
	//>>Final time measurement
	//begin_battle();
	
	p1_pokemon_data_list->shared_data = shared_data;
	p2_pokemon_data_list->shared_data = shared_data;
	printf("CHECKPOINT3\n");
	//bloquear todos excepto el primero de cada jugador
	//preguntarle al profe cual de las dos es mejor opcion
	for(int z = 1; z < AMOUNT_OF_POKEMON; ++z)
	{
		pthread_mutex_lock(&shared_data->player1_mutexes[z]);
		pthread_mutex_lock(&shared_data->player2_mutexes[z]);
	}
	printf("CHECKPOINT4\n");
	for(int z = 1; z < AMOUNT_OF_POKEMON; ++z)
	{
		pthread_mutex_lock(&p1_pokemon_data_list[z].my_mutex);
		pthread_mutex_lock(&p2_pokemon_data_list[z].my_mutex);
	}
	printf("CHECKPOINT5\n");
	battle_zone->pk1 = p1_pokemon_data_list[1];
	battle_zone->pk2 = p2_pokemon_data_list[1];
	for (int i = 0; i < AMOUNT_OF_POKEMON; ++i)
	{
		printf("CHECKPOINT6\n");
		p1_pokemon_data_list[i].shared_data = shared_data;
		p2_pokemon_data_list[i].shared_data = shared_data;
		pthread_create(&p1_pokemon[i], NULL, fight, (void *)&p1_pokemon_data_list[i]);
		pthread_create(&p2_pokemon[i], NULL, fight, (void *)&p2_pokemon_data_list[i]);
	}
	printf("CHECKPOINT7\n");
	join_threads(p1_pokemon, AMOUNT_OF_POKEMON);
	join_threads(p2_pokemon, AMOUNT_OF_POKEMON);
	printf("CHECKPOINT8\n");
	//POST BATTLE SCREEN
	//>>Print battle time on a label
	//Print winner/loser on a label
	//Print pokemon survival times on label, if pokemon didn't die, print "SURVIVED" on time label
	//Create button to go back to the selection screen
	free_memory(p1_pokemon, p2_pokemon, shared_data, p1_pokemon_data_list, p2_pokemon_data_list);
	printf("CHECKPOINT9\n");
	return 0;
}