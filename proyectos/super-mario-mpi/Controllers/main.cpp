#include "../Models/mario.cpp"
#include "mpi.h"
#include <string.h>
//aux function to validate entries
bool validate_entry(int chosen_id)
{
	if (chosen_id == 0)
	{
		return false;
	}
	else
		return true;
}

int main(int argc, char *argv[])
{
	//auxiliar variables
	int myID, numProcesses, chosenId;
	int leastCoinsID = 0;
	int mostCoinsID = 0;
	char strategy = ' ';
	//mpi init to use mpi functions
	MPI_Status status;
	MPI_Init(&argc, &argv);
	MPI_Comm_size(MPI_COMM_WORLD, &numProcesses);
	MPI_Comm_rank(MPI_COMM_WORLD, &myID);
	//auxiliar arrays for message passing
	int attackingIDs[numProcesses];
	bool activePlayers[numProcesses];
	//validation
	if (argc == 3)
	{
		if (validate_entry((size_t)strtoul(argv[1], NULL, 10)))
		{
			chosenId = (size_t)strtoul(argv[1], NULL, 10);
			strategy = *argv[2];
		}
		else
		{
			cout << "Strategy must be R L A or M, id cannot be 0" << endl;
			return 1;
		}
	}
	else
	{
		cout << "Invalid number of parameters" << endl;
		return 2;
	}
	//if my id equals chosen id print everything that is processed. execute concurrently
	if (myID == chosenId)
	{
		Mario chosen_mario(myID, chosenId, strategy, numProcesses);
		while (chosen_mario.getIsActive() == true && chosen_mario.getLocation() != 99)
		{
			const char *cstr;
			int currentCoins = chosen_mario.getCoins();
			if (chosen_mario.getWorldBlock(chosen_mario.getLocation()).empty())
			{
				if (chosen_mario.getID() == chosenId)
				{
					chosen_mario.printWalkingInfo();
					chosen_mario.printInfo();
				}
				chosen_mario.setLocation(chosen_mario.getLocation() + 1);
			}
			else
			{
				chosen_mario.setEncounterResults(chosen_mario.getEncounterOutcomes(chosen_mario.getWorldBlock(chosen_mario.getLocation())));
				if (chosen_mario.getAttack() != " ")	//if attack is not null after iteration, get attack and send it using MPI SEND
				{
					string sentAttack = chosen_mario.getAttack();
					const char *cstr = sentAttack.c_str();
					int attackingId = chosen_mario.getAttackingId();
				}
			}
			MPI_Reduce(&currentCoins, &leastCoinsID, 1, MPI_INT, MPI_MIN, 0, MPI_COMM_WORLD);	//mpi reduce to get min coins player
			MPI_Reduce(&currentCoins, &mostCoinsID, 1, MPI_INT, MPI_MAX, 0, MPI_COMM_WORLD);	//mpi reduce to get max coins player
			chosen_mario.setLeastCoinsID(leastCoinsID);											//set variables for all players so they can communicate
			chosen_mario.setMostCoinsID(mostCoinsID);
		}
		if (chosen_mario.getIsActive())	//if mario survived world run, print flag info and reset world run.
		{
			if (chosen_mario.getID() == chosenId)
			{
				chosen_mario.printFlagInfo();
				chosen_mario.printInfo();
				chosen_mario.setLocation(0);
			}
		}
		else	
		{		//if mario died in the world run, give user chance to select another mario to spectate
			cout << "Please select another mario to spectate" << endl;
			cin >> chosenId;
		}
	}
	else	//if my id is not equal to chosen id, process everything but do not print it. execute concurrently
	{
		Mario mario(myID,chosenId, numProcesses);
		while (mario.getIsActive() == true && mario.getLocation() != 99)	//iterate through the world and interact with entities
		{
			int currentCoins = mario.getCoins();
			if (mario.getWorldBlock(mario.getLocation()).empty())	//if location is empty just set location++
			{
				mario.setLocation(mario.getLocation() + 1);		
			}
			else
			{
				mario.setEncounterResults(mario.getEncounterOutcomes(mario.getWorldBlock(mario.getLocation())));	//if entities in location, process outcomes
			}
			MPI_Reduce(&currentCoins, &leastCoinsID, 1, MPI_INT, MPI_MIN, 0, MPI_COMM_WORLD);	//mpi reduce to get min coins player
			MPI_Reduce(&currentCoins, &mostCoinsID, 1, MPI_INT, MPI_MAX, 0, MPI_COMM_WORLD);	//mpi reduce to get max coins player
			mario.setLeastCoinsID(leastCoinsID);
			mario.setMostCoinsID(mostCoinsID);
		}
		if (mario.getIsActive())	//if mario survived world run, reset his location.
		{
			mario.setLocation(0);
		}
	}
	MPI_Finalize();
	return 0;
}