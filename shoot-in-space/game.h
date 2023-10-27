#pragma once

#include "application.h"

class game : public application
{
public:
    game(HINSTANCE h_instance);
    ~game();

    bool initialize() override;

private:
    void update(const timer& timer) override;
    void render(const timer& timer) override;

    void build_descriptor_heaps();
    void build_constant_buffers();
    void build_root_signature();
    void build_shaders_and_input_layout();
    void build_vertices();
    void build_pso();

private:
    Microsoft::WRL::ComPtr<ID3D12RootSignature> m_root_signature_ = nullptr;
    Microsoft::WRL::ComPtr<ID3D12PipelineState> m_pipeline_state_ = nullptr;
    Microsoft::WRL::ComPtr<ID3D12Resource> m_vertex_buffer_ = nullptr;
    D3D12_VERTEX_BUFFER_VIEW m_vbv_;

    Microsoft::WRL::ComPtr<ID3D12PipelineState> m_pso_ = nullptr;
    std::vector<D3D12_INPUT_ELEMENT_DESC> m_input_layout_;
    Microsoft::WRL::ComPtr<ID3DBlob> m_vertex_shader_;
    Microsoft::WRL::ComPtr<ID3DBlob> m_pixel_shader_;
    
};