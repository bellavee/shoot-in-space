
#include "pch.h"
#include "game.h"

using Microsoft::WRL::ComPtr;

int WINAPI wWinMain(HINSTANCE h_instance, HINSTANCE h_prev_instance, PWSTR p_cmd_line, int n_cmd_show)
{
    game game(h_instance);
    if (!game.initialize()) return 0;
    return game.run();
}

game::game(HINSTANCE h_instance) : application(h_instance) {}

game::~game() {}

bool game::initialize()
{
    if (!application::initialize()) return false;
    build_root_signature();
    build_shaders_and_input_layout();
    build_pso();
    return true;
}

void game::update(const timer& timer)
{
    
}

void game::render(const timer& timer)
{

    
}

void game::build_root_signature()
{
    D3D12_ROOT_SIGNATURE_DESC root_signature_desc = {};
    root_signature_desc.Flags = D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT;
    ComPtr<ID3DBlob> signature;
    ComPtr<ID3DBlob> error;
    D3D12SerializeRootSignature(&root_signature_desc, D3D_ROOT_SIGNATURE_VERSION_1, &signature, &error);
    m_d3d_device_->CreateRootSignature(0, signature->GetBufferPointer(), signature->GetBufferSize(), IID_PPV_ARGS(&m_root_signature_));
}

void game::build_shaders_and_input_layout()
{
    HRESULT hr = S_OK;
    
    m_vertex_shader_ = compiler_shaders(L"Shaders\\color.hlsl", "VS", "vs_5_0");
    m_pixel_shader_ = compiler_shaders(L"Shaders\\color.hlsl", "PS", "ps_5_0");

    m_input_layout_ =
    {
        { "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 },
        { "COLOR", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 12, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 }
    };
}

void game::build_pso()
{
    D3D12_GRAPHICS_PIPELINE_STATE_DESC pso_desc = {};
    pso_desc.InputLayout = { m_input_layout_.data(), (UINT)m_input_layout_.size() };
    pso_desc.pRootSignature = m_root_signature_.Get();
    pso_desc.VS = { m_vertex_shader_->GetBufferPointer(), m_vertex_shader_->GetBufferSize() };
    pso_desc.PS = { m_pixel_shader_->GetBufferPointer(), m_pixel_shader_->GetBufferSize() };
    
    pso_desc.RasterizerState = CD3DX12_RASTERIZER_DESC(D3D12_DEFAULT);
    pso_desc.BlendState = CD3DX12_BLEND_DESC(D3D12_DEFAULT);
    pso_desc.DepthStencilState = CD3DX12_DEPTH_STENCIL_DESC(D3D12_DEFAULT);
    pso_desc.SampleMask = UINT_MAX;
    
    pso_desc.PrimitiveTopologyType = D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE;
    pso_desc.RTVFormats[0] = DXGI_FORMAT_R8G8B8A8_UNORM;
    pso_desc.SampleDesc.Count = m_4_x_msaa_state_ ? 4 : 1;
    pso_desc.SampleDesc.Quality = m_4_x_msaa_state_ ? (m_4_x_msaa_quality_ - 1) : 0;
    pso_desc.SampleMask = UINT_MAX;
    pso_desc.RasterizerState = CD3DX12_RASTERIZER_DESC(D3D12_DEFAULT);
    pso_desc.BlendState = CD3DX12_BLEND_DESC(D3D12_DEFAULT);
    pso_desc.NumRenderTargets = 1;
    
    dx::throw_if_failed(m_d3d_device_->CreateGraphicsPipelineState(&pso_desc, IID_PPV_ARGS(m_pso_.ReleaseAndGetAddressOf())));
}





