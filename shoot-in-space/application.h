#pragma once

#include "pch.h"
#include "timer.h"

class application
{
protected:
    application(HINSTANCE h_instance);
    virtual ~application();

public:
    static application* get_app();
    int run();
    
    virtual bool initialize();
    virtual LRESULT msg_window_proc(HWND hwnd, UINT u_msg, WPARAM w_param, LPARAM l_param);
    
protected:
    bool init_window();
    bool init_direct3d();
    virtual void update(const timer& timer) = 0;
    virtual void render(const timer& timer) = 0;
    

    
protected:
    static application* m_app_;
    HINSTANCE       m_h_instance_   = nullptr;
    HWND            m_hwnd_         = nullptr;
    int             m_width_        = 1600;
    int             m_height_       = 900;
    const wchar_t   class_name_[15] = L"Shoot-in-space";
    LPCWSTR         m_window_name_  = L"Shoot-in-space";
    bool            m_app_paused_   = false;

    Microsoft::WRL::ComPtr<IDXGIFactory4> m_dxgi_factory_;
    Microsoft::WRL::ComPtr<IDXGISwapChain> m_swap_chain_;

    Microsoft::WRL::ComPtr<ID3D12Device> m_d3d_device_;
    Microsoft::WRL::ComPtr<ID3D12Fence> m_fence_;
    UINT64 m_current_fence_                     = 0;

    Microsoft::WRL::ComPtr<ID3D12CommandQueue> m_command_queue_;
    Microsoft::WRL::ComPtr<ID3D12CommandAllocator> m_command_alloc_;
    Microsoft::WRL::ComPtr<ID3D12CommandList> m_command_list_;

    static constexpr  UINT c_swap_buffer_count  = 2;
    UINT m_current_buffer_                      = 0;
    Microsoft::WRL::ComPtr<ID3D12Resource> m_swap_chain_buffer_[c_swap_buffer_count];
    Microsoft::WRL::ComPtr<ID3D12Resource> m_depth_stencil_buffer_;

    Microsoft::WRL::ComPtr<ID3D12DescriptorHeap> m_rtv_heap_;
    Microsoft::WRL::ComPtr<ID3D12DescriptorHeap> m_dsv_heap_;
    
    UINT m_rtv_descriptor_size_                 = 0;
    UINT m_dsv_descriptor_size_                 = 0;
    UINT m_cbv_srv_uav_descriptor_size_         = 0;

    D3D_DRIVER_TYPE m_d3d_driver_type_  = D3D_DRIVER_TYPE_HARDWARE;
    DXGI_FORMAT m_back_buffer_format_   =  DXGI_FORMAT_R8G8B8A8_UNORM;
    DXGI_FORMAT m_depth_stencil_format_ = DXGI_FORMAT_D24_UNORM_S8_UINT;

    timer m_timer_;
};
