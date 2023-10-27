﻿#pragma once
#include "Application.h"

class Application;

class Window
{
public:
    static int Run(Application* pApp, HINSTANCE hInstance, int nCmdShow);
    static HWND GetHwnd() { return m_hwnd; }

protected:
    static LRESULT CALLBACK WindowProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
    
private:
    static HWND m_hwnd;
    
};
