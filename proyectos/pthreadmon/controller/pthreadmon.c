#include "pokemon.c"
int pthreadmon()
{
	initialize_game();
	create_threads();
	finish_game();
	return 0;
}