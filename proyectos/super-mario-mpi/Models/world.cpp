#include "entities.cpp"
#define WORLD_SIZE 100
using namespace std;

class World
{
    //0 Hole, 1 Coin, 2 Little Goomba, 3 Koopa Troopa, 4 Empty, 5 Flag
    Entities world[WORLD_SIZE];

public:
    //constructor maps world consisting of string linked lists
    World()
    {
        world[8].addEntity("Coin");
        world[11].addEntity("Little Goomba");
        world[11].addEntity("Coin");
        world[11].addEntity("Coin");
        world[20].addEntity("Little Goomba");
        world[25].addEntity("Little Goomba");
        world[26].addEntity("Little Goomba");
        world[34].addEntity("Hole");
        world[40].addEntity("Little Goomba");
        world[41].addEntity("Little Goomba");
        world[43].addEntity("Hole");
        world[47].addEntity("Coin");
        world[48].addEntity("Little Goomba");
        world[49].addEntity("Little Goomba");
        world[53].addEntity("Coin");
        world[53].addEntity("Koopa Troopa");
        world[54].addEntity("Coin");
        world[56].addEntity("Coin");
        world[57].addEntity("Little Goomba");
        world[57].addEntity("Little Goomba");
        world[62].addEntity("Little Goomba");
        world[62].addEntity("Little Goomba");
        world[64].addEntity("Little Goomba");
        world[64].addEntity("Little Goomba");
        world[64].addEntity("Coin");
        world[65].addEntity("Coin");
        world[77].addEntity("Hole");
        world[85].addEntity("Coin");
        world[87].addEntity("Little Goomba");
        world[88].addEntity("Little Goomba");
        world[99].addEntity("Flag");
    }

    //gets elements in requested position or locations
    list<string> getBlockElements(int position)
    {
        return world[position].getEntities();
    }

    //sends element to requested position
    void addBlockElement(int position, string element)
    {
        world[position].addEntity(element);
    }

    //removes element from postion
    void removeBlockEement(int position, string element)
    {
        world[position].removeEntity(element);
    }

    //method for testing world integrity
    void printWorld()
    {
        for (int i = 0; i < WORLD_SIZE; i++)
        {
            list<string> entities = world[i].getEntities();
            list<string>::iterator it;
            cout << "Entites on block # " << i << ": ";
            for (it = entities.begin(); it != entities.end(); ++it)
            {
                cout << *it << " ";
            }
            cout << endl;
        }
    }
};