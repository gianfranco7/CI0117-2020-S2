#include <iostream>
#include <vector>
#include <map>
#include "../Models/world.cpp"
using namespace std;
//Class that simulates a player that the user can take control of or not

class Mario
{
    int id;
    int chosenid;
    int attackingId;
    int attackerId;
    int mostCoinsId;
    int leastCoinsId;
    int location;
    int coins;
    int totalPlaying;
    bool isActive;
    char strategy;
    string attack;
    World world;

public:
    Mario(int playerid, int chosenId, int totalPlayers)
    {
        id = playerid;
        location = 0;
        chosenid = chosenId;
        coins = 0;
        attackingId = 0;
        attackerId = 0;
        isActive = true;
        strategy = getStrategy();
        totalPlaying = totalPlayers;
        attack = " ";
    }

    Mario(int playerid, int chosenId, char chosen_strategy, int totalPlayers)
    {
        id = playerid;
        chosenid = chosenId;
        location = 0;
        coins = 0;
        attackingId = 0;
        attackerId = 0;
        isActive = true;
        strategy = chosen_strategy;
        totalPlaying = totalPlayers;
        attack = " ";
    }

    void setTarget()
    {
        if (strategy == 'R')
        {
            attackingId = getRandomTarget();
        }
        if (strategy == 'L')
        {
            //ATAQUE AL QUE TIENE MENOS MONEDAS
            attackingId = leastCoinsId;
        }
        if (strategy == 'M')
        {
            //ATAQUE AL QUE TIENE MAS MONEDAS
            attackingId = mostCoinsId;
        }
        if (strategy == 'A')
        {
            //ATAQUE A ATACANTE O RANDOM
            attackingId = getRandomTarget();
        }
    }

    string getAttack()
    {
        return attack;
    }

    int getID()
    {
        return id;
    }

    int getAttackingId()
    {
        return attackingId;
    }

    void setMostCoinsID(int id)
    {
        mostCoinsId = id;
    }

    void setLeastCoinsID(int id)
    {
        leastCoinsId = id;
    }

    int getLocation()
    {
        return location;
    }

    void setLocation(int newLocation)
    {
        location = newLocation;
    }

    int getCoins()
    {
        return coins;
    }

    bool getIsActive()
    {
        return isActive;
    }

    void setIsActive(bool active)
    {
        isActive = active;
    }

    int generateRandomInt(int lowerLimit, int upperLimit)
    {
        srand(time(NULL));
        return (rand() % (upperLimit + 1)) + lowerLimit;
    }

    char getStrategy()
    {
        int choice = generateRandomInt(0, 3);
        switch (choice)
        {
        case 0:
            return 'R';
        case 1:
            return 'L';
        case 2:
            return 'M';
        case 3:
            return 'A';
        }
    }

    int getRandomTarget()
    {
        return generateRandomInt(0, totalPlaying);
    }

    void printAttacker()
    {
        cout << " being attacked by #" << attackerId;
    }

    void printAttacking()
    {
        cout << "atacking #" << attackingId;
    }

    void printDivisor()
    {
        cout << " | ";
    }

    void printStrategy()
    {
        cout << " attack strategy: ";
        if (strategy == 'R')
        {
            cout << "RANDOM ";
        }
        if (strategy == 'L')
        {
            cout << "LESS COINS ";
        }
        if (strategy == 'M')
        {
            cout << "MORE COINS ";
        }
        if (strategy == 'A')
        {
            cout << "ATTACKER ";
        }
    }

    void printTotalPlayers()
    {
        cout << "Total playing: " << totalPlaying << endl;
    }

    //Prints all info in requested format
    void printInfo()
    {
        printDivisor();
        printAttacking();
        printDivisor();
        printAttacker();
        printDivisor();
        printStrategy();
        printDivisor();
        printTotalPlayers();
    }

    //Gets entities in a world block
    list<string> getWorldBlock(int position)
    {
        list<string> entities = world.getBlockElements(position);
        return entities;
    }

    //gets a multimap of the event outcome <outcome code, entity>
    multimap<int, string> getEncounterOutcomes(list<string> blockEntities)
    {
        multimap<int, string> outcomes;
        list<string>::iterator it;
        for (it = blockEntities.begin(); it != blockEntities.end(); ++it)
        {
            string entity = *it;
            int outcome = -1;
            if (entity == "Coin")
            {
                outcome = getCoinEncounterOutcome();
                outcomes.insert(pair<int, string>(outcome, entity));
            }
            if (entity == "Hole")
            {
                outcome = getHoleEncounterOutcome();
                outcomes.insert(pair<int, string>(outcome, entity));
            }
            if (entity == "Little Goomba")
            {
                outcome = getLittleGoombaEncounterOutcome();
                outcomes.insert(pair<int, string>(outcome, entity));
            }
            if (entity == "Koopa Troopa")
            {
                outcome = getKoopaTroopaEncounterOutcome();
                outcomes.insert(pair<int, string>(outcome, entity));
            }
        }
        return outcomes;
    }

    //recieves attack entity and adds it to world in location+10 as per requirements
    void recieveAttack(string attack)
    {
        if (attack == "Little Goomba")
        {
            world.addBlockElement(location + 10, attack);
        }
        if (attack == "Koopa Troopa")
        {
            world.addBlockElement(location + 10, attack);
        }
    }

    //sets attack to send
    string setAttack(string sentAttack)
    {
        attack = sentAttack;
    }

    //iterates through outcomes multimap and sets the results in the world and the player
    void setEncounterResults(multimap<int, string> outcomes)
    {
        multimap<int, string>::iterator it;
        for (it = outcomes.begin(); it != outcomes.end(); ++it)
        {
            if (isActive)
            {
                if (it->second == "Coin")
                {
                    switch (it->first)
                    {
                    case 0:
                        if (id == chosenid)
                        {
                            cout << "World pos. " << location << ": ";
                            cout << "Mario #" << id << " didn't jump and left a coin. ";
                            cout << "Coins: " << coins;
                            printInfo();
                        }
                        break;
                    case 1:

                        ++coins;
                        world.removeBlockEement(location, "Coin");
                        if (id == chosenid)
                        {
                            cout << "World pos. " << location << ": ";
                            cout << "Mario #" << id << " jumped and grabbed a coin. ";
                            cout << "Coins: " << coins;
                            printInfo();
                        }
                        break;
                    }
                }
                if (it->second == "Hole")
                {
                    switch (it->first)
                    {
                    case 0:
                        isActive = false;
                        if (id == chosenid)
                        {
                            cout << "World pos. " << location << ": ";
                            cout << "Mario #" << id << " didn't jump and fell to his death. " << endl;
                        }
                        break;
                    case 1:
                        if (id == chosenid)
                        {
                            cout << "World pos. " << location << ": ";
                            cout << "Mario #" << id << " jumped and kept walking. ";
                            cout << "Coins: " << coins;
                            printInfo();
                        }
                        break;
                    }
                }
                if (it->second == "Little Goomba")
                {
                    switch (it->first)
                    {
                    case 0:
                        if (id == chosenid)
                        {
                            cout << "World pos. " << location << ": ";
                            cout << "Mario #" << id << " didn't jump and was killed by a little goomba. " << endl;
                        }
                        isActive = false;
                        break;
                    case 1:
                        if (id == chosenid)
                        {
                            cout << "World pos. " << location << ": ";
                            cout << "Mario #" << id << " jumped and avoided the little goomba. ";
                            cout << "Coins: " << coins;
                            printInfo();
                        }
                        break;
                    case 2:
                        world.removeBlockEement(location, "Little Goomba");
                        setAttack("Little Goomba");
                        if (id == chosenid)
                        {
                            cout << "World pos. " << location << ": ";
                            cout << "Mario #" << id << " jumped and killed the little goomba. ";
                            cout << "Coins: " << coins;
                            printInfo();
                        }
                        break;
                    }
                }
                if (it->second == "Koopa Troopa")
                {
                    switch (it->first)
                    {
                    case 0:
                        isActive = false;
                        if (id == chosenid)
                        {
                            cout << "World pos. " << location << ": ";
                            cout << "Mario #" << id << " didn't jump and was killed by a koopa troopa. " << endl;
                        }
                        break;
                    case 1:
                        if (id == chosenid)
                        {
                            cout << "World pos. " << location << ": ";
                            cout << "Mario #" << id << " jumped and avoided the koopa troopa. ";
                            cout << "Coins: " << coins;
                            printInfo();
                        }
                        break;
                    case 2:
                        world.removeBlockEement(location, "Koopa Troopa");
                        setAttack("Koopa Troopa");
                        if (id == chosenid)
                        {
                            cout << "World pos. " << location << ": ";
                            cout << "Mario #" << id << " jumped and killed the koopa troopa. ";
                            cout << "Coins: " << coins;
                            printInfo();
                        }
                        break;
                    }
                }
            }
        }
        if (isActive)
            ++location;
    }

    //prints only walking info
    void printWalkingInfo()
    {
        cout << "World pos. " << location << ": ";
        cout << "Mario #" << id << " is walking. ";
        cout << "Coins: " << coins;
    }

    //prints info in case of flag encounter
    void printFlagInfo()
    {
        cout << "World pos. " << location << ": ";
        cout << "Mario #" << id << " made it to the flag! ";
        cout << "Total Coins: " << coins;
    }

    //generates random probability
    bool calculateProbabilityResult(int probability)
    {
        return (rand() % 100) < probability;
    }

    //recieves probability vectors and returns weighted probability
    int calculateEncounterResult(vector<double> probs)
    {
        int randomNumber = generateRandomInt(1, 100), upperLimit = 0;
        for (__SIZE_TYPE__ i = 0; i < probs.size(); ++i)
        {
            upperLimit = upperLimit + (probs[i] * 100);
            if (randomNumber <= upperLimit)
            {
                return i;
            }
        }
        return 0;
    }

    //sets weighted probabilities in vector and sends them to method for processing
    int getCoinEncounterOutcome()
    {
        vector<double> probabilities = {0.5, 0.5};
        return calculateEncounterResult(probabilities);
    }

    //sets weighted probabilities in vector and sends them to method for processing
    int getHoleEncounterOutcome()
    {
        vector<double> probabilities = {0.05, 0.95};
        return calculateEncounterResult(probabilities);
    }

    //sets weighted probabilities in vector and sends them to method for processing
    int getLittleGoombaEncounterOutcome()
    {
        vector<double> probabilities = {0.05, 0.55, 0.40};
        return calculateEncounterResult(probabilities);
    }

    //sets weighted probabilities in vector and sends them to method for processing
    int getKoopaTroopaEncounterOutcome()
    {
        vector<double> probabilities = {0.10, 0.53, 0.37};
        return calculateEncounterResult(probabilities);
    }
};