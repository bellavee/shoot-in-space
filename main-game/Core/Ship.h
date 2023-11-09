#pragma once
#include "helper.h"

class Ship
{
	Vector3f position;
	Vector3f velocity;
public:
	std::string mesh;
	int HP;
	Ship(std::string mesh, Vector3f position, Vector3f velocity, int HP);
	float distanceFromPosition(Vector3f inPosition);
	Vector3f getPosition();
	void setPosition(Vector3f position);
};