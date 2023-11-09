#include "Asteroide.h"

Asteroide::Asteroide(std::string mesh, Vector3f position, Vector3f velocity, int HP) : mesh(mesh), position(position), velocity(velocity), HP(HP)
{
}

float Asteroide::distanceFromPosition(Vector3f inPosition)
{
	float distance = sqrt((position.x - inPosition.x) + (position.y - inPosition.y) + (position.z - inPosition.z));
	return distance;
}

Vector3f Asteroide::updatePosition(float dt)
{
	position.x += velocity.x * dt;
	position.y += velocity.y * dt;
	position.z += velocity.z * dt;

	return position;
}

Vector3f Asteroide::getPosition()
{
	return position;
}