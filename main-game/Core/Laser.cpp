#include "Laser.h"

Laser::Laser(std::string mesh, Vector3f position, Vector3f velocity) : mesh(mesh), position(position), velocity(velocity)
{
}

float Laser::distanceFromPosition(Vector3f inPosition)
{
	float distance = sqrt((position.x - inPosition.x) + (position.y - inPosition.y) + (position.z - inPosition.z));
	return distance;
}

Vector3f Laser::updatePosition(float dt)
{
	position.x += velocity.x * dt;
	position.y += velocity.y * dt;
	position.z += velocity.z * dt;

	return position;
}

Vector3f Laser::getPosition()
{
	return position;
}