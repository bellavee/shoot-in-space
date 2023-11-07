#include "Entity.h"

unsigned int Entity::idCounter = 0; // Initialize static member

Entity::Entity(std::string name)
    : _id(++idCounter), _name(name) // Use pre-increment to assign and then increase the id
{
}

Entity::~Entity() {}
