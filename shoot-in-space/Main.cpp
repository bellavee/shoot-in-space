
#include "Graphics/Game.h"
#include "Utils/pch.h"
#include "Window/Window.h"

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE, LPSTR, int nCmdShow)
{
    Game game(1920, 1080, L"Shoot-in-space");
    return Window::Run(&game, hInstance, nCmdShow);
}