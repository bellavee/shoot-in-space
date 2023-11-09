#include <d3d12.h>
#include "Window.h"

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE prevInstance,
	PSTR cmdLine, int showCmd)
{
	Window::getInstance()->CreateGameWin(L"Window", 500, 500);
	
}