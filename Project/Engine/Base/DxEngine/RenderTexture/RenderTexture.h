
#pragma once
#include <d3d12.h>
#include "wrl.h"
#include <cstdint>

#include "../../../Math/Structure/Vector4.h"
#include "Matrix4x4.h"

using Microsoft::WRL::ComPtr;

/// <summary>
/// RenderTextureクラス
/// </summary>
class RenderTexture
{
public:
    // static helper to create a texture resource
    static ComPtr<ID3D12Resource> CreateResource(ComPtr<ID3D12Device> device, uint32_t width, uint32_t height, DXGI_FORMAT format, const NumaEngine::Vector4& clearColor);

    // initialize internal resources
    void Initialize();

    // barriers and draw
    void StartBarrier();
    void EndBarrier();
    void PreDraw();
    void Draw();

    // accessors
    ID3D12Resource* GetDSVResource() const;
    D3D12_CPU_DESCRIPTOR_HANDLE GetDSVHandle() const;

    uint32_t GetFinalSrvIndex() const { return finalSrvIndex_; }
    void SetFinalSrvIndex(uint32_t idx) { finalSrvIndex_ = idx; }

private:
    // resources
    ComPtr<ID3D12Resource> depthStencilResource_ = nullptr;
    uint32_t depthIndex_ = 0;

    ComPtr<ID3D12Resource> renderTextureResource_ = nullptr;
    D3D12_CPU_DESCRIPTOR_HANDLE renderTextureHandle_{};
    uint32_t renderTextureSRVIndex_ = 0;

    // pipeline
    ComPtr<ID3D12RootSignature> rootSignature_ = nullptr;
    ComPtr<ID3D12PipelineState> pipelineState_ = nullptr;

    // barrier
    D3D12_RESOURCE_BARRIER barrier_{};

    // last final SRV index
    uint32_t finalSrvIndex_ = 0;
};

