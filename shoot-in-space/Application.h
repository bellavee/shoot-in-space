#pragma once

#include "pch.h"
#include "Timer.h"

class Application
{
public:
    Application(UINT width, UINT height, std::wstring name);
    virtual ~Application();

    virtual void OnInit() = 0;
    virtual void OnUpdate() = 0;
    virtual void OnRender() = 0;
    virtual void OnDestroy() = 0;

    virtual void OnKeyDown(UINT8)   {}
    virtual void OnKeyUp(UINT8)     {}
    
    UINT GetWidth() const           { return m_width; }
    UINT GetHeight() const          { return m_height; }
    const WCHAR* GetTitle() const   { return m_title.c_str(); }

protected:
    UINT m_width;
    UINT m_height;
    float m_aspectRatio;
    bool m_useWarpDevice;

private:
    std::wstring m_assetPath;
    std::wstring m_title;
    
};
