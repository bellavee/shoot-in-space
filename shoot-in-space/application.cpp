
#include "application.h"

LRESULT CALLBACK window_proc(HWND hwnd, UINT u_msg, WPARAM w_param, LPARAM l_param)
{
    return application::get_app()->msg_window_proc(hwnd, u_msg, w_param, l_param);
};

// ----- Application function -----

application*::application::m_app_ = nullptr;
application::application(HINSTANCE h_instance)
{
    this->m_h_instance_ = h_instance;
    assert(m_app_ == nullptr);
    m_app_ = this;
}

application::~application() {}

application* application::get_app()
{
    return m_app_;
}

int application::run()
{
    MSG msg = {};
    while (WM_QUIT != msg.message)
    {
        if (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE))
        {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        } else
        {
            m_timer_.tick();
            if (!m_app_paused_)
            {
                update(m_timer_);
                render(m_timer_);
            }
        }
    }
    return static_cast<int>(msg.wParam);
}


LRESULT application::msg_window_proc(const HWND hwnd, const UINT u_msg, const WPARAM w_param, const LPARAM l_param)
{
    switch (u_msg)
    {
    case WM_DESTROY:
        PostQuitMessage(0);
        return (0);

    case WM_ACTIVATE:
        if (LOWORD(w_param) == WA_INACTIVE)
        {
            m_app_paused_ = true;
        }
    default:
        break;
    }
    return DefWindowProc(hwnd, u_msg, w_param, l_param);
}

bool application::initialize()
{
    if (!init_window()) return false;
    if (!init_direct3d()) return false;
    return true;
}

bool application::init_window()
{
    // Register window class
    WNDCLASS wc = {};
    wc.lpfnWndProc      = window_proc;
    wc.lpszClassName    = class_name_;
    wc.hInstance        = m_h_instance_;

    if (!RegisterClass(&wc)) return false;
    
    m_hwnd_ = CreateWindowEx(
    0,
    class_name_,
    m_window_name_,
    WS_OVERLAPPEDWINDOW,

    CW_USEDEFAULT, CW_USEDEFAULT, m_width_, m_height_,

    nullptr,
    nullptr,
    m_h_instance_,
    nullptr
    );

    if (m_hwnd_ == nullptr) return false;

    ShowWindow(m_hwnd_, SW_SHOW);
    UpdateWindow(m_hwnd_);
    
    return true;
}

bool application::init_direct3d()
{
    return true;
}



