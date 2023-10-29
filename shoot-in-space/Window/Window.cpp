#include "Window.h"
#include "../Utils/pch.h"

HWND Window::m_hwnd = nullptr;
LARGE_INTEGER Window::m_frequency;
LARGE_INTEGER Window::m_lastTime;
double Window::m_deltaTime = 0.0;

int Window::Run(Manager* pAppManager, HINSTANCE hInstance, int nCmdShow)
{
    WNDCLASSEX windowClass = { 0 };
    windowClass.cbSize          = sizeof(WNDCLASSEX);
    windowClass.style           = CS_HREDRAW | CS_VREDRAW;
    windowClass.lpfnWndProc     = WindowProc;
    windowClass.hInstance       = hInstance;
    windowClass.hCursor         = LoadCursor(nullptr, IDC_ARROW);
    windowClass.lpszClassName   = L"ApplicationClass";
    RegisterClassEx(&windowClass);
    
    RECT windowRect = { 0, 0,
        static_cast<LONG>(pAppManager->GetWidth()),
        static_cast<LONG>(pAppManager->GetHeight()) };
    AdjustWindowRect(&windowRect, WS_OVERLAPPEDWINDOW, FALSE);

    m_hwnd = CreateWindow(
        windowClass.lpszClassName,
        pAppManager->GetTitle(),
        WS_OVERLAPPEDWINDOW,
        CW_USEDEFAULT,
        CW_USEDEFAULT,
        windowRect.right - windowRect.left,
        windowRect.bottom - windowRect.top,
        nullptr,
        nullptr,
        hInstance,
        pAppManager);

    pAppManager->OnInit();
    ShowWindow(m_hwnd, nCmdShow);

    QueryPerformanceFrequency(&m_frequency);
    QueryPerformanceCounter(&m_lastTime);
    
    MSG msg = {};
    while (msg.message != WM_QUIT)
    {
        LARGE_INTEGER currentTime;
        QueryPerformanceCounter(&currentTime);
        m_deltaTime = static_cast<double>(currentTime.QuadPart - m_lastTime.QuadPart) / static_cast<double>(m_frequency.QuadPart);
        m_lastTime = currentTime;
        
        if (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE))
        {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
    }

    pAppManager->OnDestroy();

    return static_cast<char>(msg.wParam);
}

LRESULT CALLBACK Window::WindowProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    Manager* pAppManager = reinterpret_cast<Manager*>(GetWindowLongPtr(hWnd, GWLP_USERDATA));

    switch (message)
    {
    case WM_CREATE:
        {
            LPCREATESTRUCT pCreateStruct = reinterpret_cast<LPCREATESTRUCT>(lParam);
            SetWindowLongPtr(hWnd, GWLP_USERDATA, reinterpret_cast<LONG_PTR>(pCreateStruct->lpCreateParams));
        }
        return 0;

    case WM_KEYDOWN:
        if (pAppManager)
        {
            pAppManager->OnKeyDown(static_cast<UINT8>(wParam));
        }
        return 0;

    case WM_KEYUP:
        if (pAppManager)
        {
            pAppManager->OnKeyUp(static_cast<UINT8>(wParam));
        }
        return 0;

    case WM_PAINT:
        if (pAppManager)
        {
            pAppManager->OnUpdate(m_deltaTime);
            pAppManager->OnRender();
        }
        return 0;

    case WM_DESTROY:
        PostQuitMessage(0);
        return 0;
    default:
        break;
    }

    // Handle any messages the switch statement didn't.
    return DefWindowProc(hWnd, message, wParam, lParam);
}

