
#include "Game.h"
#include "pch.h"
#include "Window.h"

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE, LPSTR, int nCmdShow)
{
    Game game(1600, 900, L"Shoot-in-space");
    return Window::Run(&game, hInstance, nCmdShow);
}