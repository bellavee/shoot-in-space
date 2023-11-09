#pragma once
#include <string>
#include <vector>
#include <Windows.h>
#include <stdlib.h>
#include <time.h>

struct Vector3f {
	float x;
	float y;
	float z;
	Vector3f(float x, float y, float z) : x(x), y(y), z(z) {}
	Vector3f operator +(Vector3f b) {
		return Vector3f(x + b.x, y + b.y, z + b.z);
	}
	Vector3f operator -(Vector3f b) {
		return Vector3f(x - b.x, y - b.y, z - b.z);
	}
};

class Timer
{
	float s_performTime;
	float s_frequency;
public:
	void InitSystemTime();
	float GetSystemTimeEx();
};