#pragma once
#include <Windows.h>
#include "Engine.h"
#include <d3d12.h>
#include <optional>
#include <memory>
#include "Mouse.h"
#include "GUI.h"

#pragma comment(lib, "user32.lib")
#pragma comment(lib, "Gdi32.lib")


class Window
{
public:
	Window();
	~Window();
	static std::shared_ptr < Window > getInstance();
	HWND CALLBACK CreateGameWin(LPCWSTR, int, int);
	GUI graphics;

protected:
	Mouse mouse;

private:
	static LRESULT CALLBACK WndProcess(HWND, UINT, WPARAM, LPARAM);
	static std::shared_ptr<Window>_instance;
	int CALLBACK WinMain(HINSTANCE, HINSTANCE, LPSTR, INT);
	void InitVariables(int, int);
	void CreateWinClass();
	void MessageLoop();


	HWND InitWin(LPCWSTR);
	WNDCLASSEX wc;
	bool Close;
	int _winwidth;
	int _winheight;
};