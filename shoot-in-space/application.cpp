
#include "application.h"

using Microsoft::WRL::ComPtr;

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
            m_timer_.stop();
        } else
        {
            m_app_paused_ = false;
            m_timer_.start();
        }
        return 0;
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
    dx::throw_if_failed(CreateDXGIFactory1(IID_PPV_ARGS(m_dxgi_factory_.ReleaseAndGetAddressOf())));
    
    dx::throw_if_failed(D3D12CreateDevice(
        nullptr,
        D3D_FEATURE_LEVEL_12_0,
        IID_PPV_ARGS(m_d3d_device_.ReleaseAndGetAddressOf())));

    dx::throw_if_failed(m_d3d_device_->CreateFence(0, D3D12_FENCE_FLAG_NONE, IID_PPV_ARGS(&m_fence_)));

    m_rtv_descriptor_size_ = m_d3d_device_->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_RTV);
    m_dsv_descriptor_size_ = m_d3d_device_->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_DSV);
    m_cbv_srv_uav_descriptor_size_ = m_d3d_device_->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV);
    
    return true;
}

void application::create_command_objects()
{
    D3D12_COMMAND_QUEUE_DESC queue_desc = {};
    queue_desc.Type = D3D12_COMMAND_LIST_TYPE_DIRECT;
    queue_desc.Flags = D3D12_COMMAND_QUEUE_FLAG_NONE;

    dx::throw_if_failed(m_d3d_device_->CreateCommandQueue(&queue_desc, IID_PPV_ARGS(m_command_queue_.ReleaseAndGetAddressOf())));

    dx::throw_if_failed(m_d3d_device_->CreateCommandAllocator(D3D12_COMMAND_LIST_TYPE_DIRECT, IID_PPV_ARGS(m_command_alloc_.ReleaseAndGetAddressOf())));

    dx::throw_if_failed(m_d3d_device_->CreateCommandList(
        0,
        D3D12_COMMAND_LIST_TYPE_DIRECT,
        m_command_alloc_.Get(),
        nullptr,
        IID_PPV_ARGS(m_command_list_.GetAddressOf())));

   m_command_list_->Close();
}

void application::create_swap_chain()
{
    m_swap_chain_.Reset();
    DXGI_SWAP_CHAIN_DESC sd;
    sd.BufferDesc.Width = m_width_;
    sd.BufferDesc.Height = m_height_;
    sd.BufferDesc.RefreshRate.Numerator = 60;
    sd.BufferDesc.RefreshRate.Denominator = 1;
    sd.BufferDesc.Format = m_back_buffer_format_;
    sd.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
    sd.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;
    sd.SampleDesc.Count = m_4_x_msaa_state_ ? 4 : 1;
    sd.SampleDesc.Quality = m_4_x_msaa_state_ ? (m_4_x_msaa_quality_ - 1) : 0;
    sd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
    sd.BufferCount = c_swap_buffer_count;
    sd.OutputWindow = m_hwnd_;
    sd.Windowed = true;
    sd.SwapEffect = DXGI_SWAP_EFFECT_FLIP_DISCARD;
    sd.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;
    
    dx::throw_if_failed(m_dxgi_factory_->CreateSwapChain(
        m_command_queue_.Get(),
        &sd,
        m_swap_chain_.GetAddressOf()));
}

void application::flush_command_queue()
{
    m_current_fence_++;
    dx::throw_if_failed(m_command_queue_->Signal(m_fence_.Get(), m_current_fence_));

    if (m_fence_->GetCompletedValue() < m_current_fence_)
    {
        HANDLE event_handle = CreateEventEx(nullptr, nullptr, 0, EVENT_ALL_ACCESS);
        dx::throw_if_failed(m_fence_->SetEventOnCompletion(m_current_fence_, event_handle));

        WaitForSingleObject(event_handle, INFINITE);
        CloseHandle(event_handle);
    }
}

ID3D12Resource* application::get_curr_back_buffer() const
{
    return m_swap_chain_buffer_[m_curr_back_buffer_].Get();
}

D3D12_CPU_DESCRIPTOR_HANDLE application::get_curr_back_buffer_view() const
{
    return CD3DX12_CPU_DESCRIPTOR_HANDLE(m_rtv_heap_->GetCPUDescriptorHandleForHeapStart(), m_curr_back_buffer_, m_rtv_descriptor_size_);
}

D3D12_CPU_DESCRIPTOR_HANDLE application::get_dsv() const
{
    return m_dsv_heap_->GetCPUDescriptorHandleForHeapStart();
}

Microsoft::WRL::ComPtr<ID3DBlob> application::compiler_shaders(const WCHAR* filename, const char* entry_point, const char* targer)
{
    UINT compile_flags = 0;
    ComPtr<ID3DBlob> bytecode;
    ComPtr<ID3DBlob> errors;

    HRESULT hr = D3DCompileFromFile(
        filename,
        nullptr,
        nullptr,
        entry_point,
        targer,
        compile_flags,
        0,
        &bytecode,
        &errors
        );

    if (errors != nullptr) OutputDebugStringA(static_cast<char*>(errors->GetBufferPointer()));

    if (FAILED(hr)) throw std::runtime_error("Failed to compile shader.");

    return bytecode;
}










