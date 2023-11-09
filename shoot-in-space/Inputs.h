#pragma once
#include <Windows.h>
struct Float2
{
	float x;
	float y;
};
class Inputs
{
public:
	POINT mPoint;
	static Float2 GetMousePos(HWND window);
};

