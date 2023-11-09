#pragma once
#include "helper.h"

class Keyboard
{
public:
	static bool isKeyPressed(int key) { return (GetKeyState(key) & 0x8000); }
};