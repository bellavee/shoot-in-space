#include "Ship.h"

Ship::Ship(std::string mesh, Vector3f position, Vector3f velocity, int HP) : mesh(mesh), position(position), velocity(velocity), HP(HP)
{
}

float Ship::distanceFromPosition(Vector3f inPosition)
{
	float distance = sqrt((position.x - inPosition.x) + (position.y - inPosition.y) + (position.z - inPosition.z));
	return distance;
}

Vector3f Ship::getPosition()
{
	return position;
}


void Ship::setPosition(Vector3f position)
{
	this->position = position;
}