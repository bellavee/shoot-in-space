#include "Inputs.h"

Float2 Inputs::GetMousePos(HWND window)
{
	GetCursorPos(&mPoint);
	ScreenToClient(window, &mPoint);
	Float2 pos = { mPoint.x, mPoint.y };
	return pos;
};