#include <Windows.h>
#include "Window.h"
#include <sstream>

std::shared_ptr<Window> Window::_instance = nullptr;
WCHAR WndClassName[MAX_NAME_STRING] = L"EngineWndClass";
GUI graphics;


Window::Window() {

	wc = {};
	_instance = nullptr;
	Close = false;
}

Window::~Window() {}

LRESULT CALLBACK WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	switch (msg)
	{
	case WM_CLOSE:
		PostQuitMessage(69);
		break;

	default:
		return DefWindowProc(hWnd, msg, wParam, lParam);
	}
	return 0;
}

void Window::InitVariables(int width, int height)
{
	_winheight = height;
	_winwidth = width;
}

void Window::CreateWinClass()
{

	WNDCLASSEX wc = { sizeof(wc) };
	wc.style = CS_OWNDC;
	wc.lpfnWndProc = WndProc;
	wc.cbClsExtra = 0;
	wc.cbWndExtra = 0;

	wc.hInstance = HInstance();
	wc.hIcon = nullptr;

	wc.hCursor = nullptr;
	wc.hbrBackground = nullptr;

	wc.lpszMenuName = nullptr;
	wc.lpszClassName = WndClassName;

	wc.hIconSm = nullptr;
	RegisterClassEx(&wc);
}

HWND Window::CreateGameWin(LPCWSTR name, int width, int height)
{
	HWND returnWin;

	CreateWinClass();
	InitVariables(width, height);
	returnWin = InitWin(name);
	MessageLoop();
	return returnWin;
}

HWND Window::InitWin(LPCWSTR name)
{
	HWND hWnd = CreateWindow(

		WndClassName, name,
		WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT,
		CW_USEDEFAULT,
		_winwidth,
		_winheight,
		nullptr, //parent window
		nullptr, //menus
		HInstance(),
		this,

		);
	if (!hWnd)
	{
		MessageBox(NULL, L"CreateWindow Failed!", L"Error", NULL);
		PostQuitMessage(0);
	}
	//show the window
	//if (!graphics.Init(hWnd))
	//{
	//	delete &graphics;
	//	return false;
	//}
	ShowWindow(hWnd, SW_SHOW);
	return hWnd;
}

void Window::MessageLoop()
{
	MSG msg = { 0 };

	if (msg.message == WM_QUIT)
	{
		Close = true;
	}

	while (msg.message != WM_QUIT)
	{
		if (Close == true)
		{
			return;
		}
		while (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg); //send msg to WndProc
		}
	}
}

std::shared_ptr<Window> Window::getInstance()
{
	if (_instance == nullptr)
	{
		std::shared_ptr<Window> ptr(new Window);
		_instance = ptr;
	}
	return _instance;
}

