#pragma once
#include "helper.h"

class Laser
{
	Vector3f position;
	Vector3f velocity;
public:
	std::string mesh;
	Laser(std::string mesh, Vector3f position, Vector3f velocity);
	float distanceFromPosition(Vector3f inPosition);
	Vector3f updatePosition(float dt);
	Vector3f getPosition();
};