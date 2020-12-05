#include <list>
#include <iostream>
#include <algorithm>
using namespace std;
class Entities
{
    list<string> entities;
    list<string>::iterator it;

public:

    Entities()
    {
        it = entities.begin();
    }

    ~Entities()
    {
        entities.clear();
    }

    //adds entity to list
    void addEntity(string entity)
    {
        entities.insert(it, entity);
    }

    //gets entities list
    list<string> getEntities()
    {
        return entities;
    }

    //removes specific entity from list
    void removeEntity(string entity)
    {
        list<string>::iterator it = find(entities.begin(), entities.end(), entity);
        entities.erase(it);
    }
};