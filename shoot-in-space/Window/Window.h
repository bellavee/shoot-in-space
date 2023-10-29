#pragma once
#include "../Graphics/Manager.h"

class Manager;

class Window
{
public:
    static int Run(Manager* pAppManager, HINSTANCE hInstance, int nCmdShow);
    static HWND GetHwnd() { return m_hwnd; }

protected:
    static LRESULT CALLBACK WindowProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
    
private:
    static HWND m_hwnd;
    static LARGE_INTEGER m_frequency;
    static LARGE_INTEGER m_lastTime;
    static double m_deltaTime;
    
};
