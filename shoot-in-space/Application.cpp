
#include "Application.h"

using Microsoft::WRL::ComPtr;


Application::Application(UINT width, UINT height, std::wstring name)
{
    m_width = width;
    m_height = height;
    m_title = name;
    m_useWarpDevice = false;
    m_aspectRatio = width / height;
}

Application::~Application()
{
    
}












