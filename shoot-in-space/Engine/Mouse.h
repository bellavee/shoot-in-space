#pragma once
#include <queue>
#include <map>
#include <d3d12.h>


enum class Type
{
	LPress,
	LRelease,
	Move,
	Invalid
};
class Mouse
{
public:
	Mouse();
	void onMouseDown(WPARAM btnState, int x, int y);
	void onMouseMove(/*WPARAM btnState, */int newx, int newy);
	bool leftPressed(char);
	int getPosX();
	int getPosY();
	std::pair<int, int> GetPos();

private:
	int x;
	int y;
	POINT mousePos;
	std::map<int, Type> mMouseStates;
};
