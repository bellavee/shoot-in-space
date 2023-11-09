#include "Entity.h"

unsigned int Entity::idCounter = 0; // Initialize static member

Entity::Entity() : _id(++idCounter)// Use pre-increment to assign and then increase the id
{
}

Entity::~Entity() {}
