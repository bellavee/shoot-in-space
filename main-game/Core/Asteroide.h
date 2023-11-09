#pragma once
#include "helper.h"

class Asteroide
{
	Vector3f position;
	Vector3f velocity;
public:
	std::string mesh;
	int HP;
	Asteroide(std::string mesh, Vector3f position, Vector3f velocity, int HP);
	float distanceFromPosition(Vector3f inPosition);
	Vector3f updatePosition(float dt);
	Vector3f getPosition();
};