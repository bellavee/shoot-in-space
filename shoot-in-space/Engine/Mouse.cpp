#include "Mouse.h"



Mouse::Mouse()
{
}

void Mouse::onMouseDown(WPARAM btnState, int x, int y)
{
	mousePos.x = x;
	mousePos.y = y;
}

void Mouse::onMouseMove(/*WPARAM btnState,*/ int newx, int newy)
{
	mousePos.x = newx;
	mousePos.y = newy;
}

bool Mouse::leftPressed(char mCode)
{
	return (mMouseStates[mCode] == Type::LPress);
}

int Mouse::getPosX()
{
	return x;
}

int Mouse::getPosY()
{
	return y;
}
std::pair<int, int> Mouse::GetPos()
{
	return {x,y};
}