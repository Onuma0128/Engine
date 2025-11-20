#include "ShadowMap.h"

#include "DirectXEngine.h"
#include "RenderTexture.h"
#include "SrvManager.h"
#include "DsvManager.h"
#include "TextureManager.h"

void ShadowMap::CreateShadowMap(uint32_t size)
{
    shadowSize_ = size;

	// シャドウマップ用テクスチャの作成
    D3D12_RESOURCE_DESC desc{};
    desc.Dimension = D3D12_RESOURCE_DIMENSION_TEXTURE2D;
    desc.Width = size;
    desc.Height = size;
    desc.DepthOrArraySize = 1;
    desc.MipLevels = 1;
    desc.Format = DXGI_FORMAT_R24G8_TYPELESS;
    desc.SampleDesc.Count = 1;
    desc.Flags = D3D12_RESOURCE_FLAG_ALLOW_DEPTH_STENCIL;

    D3D12_CLEAR_VALUE clear{};
    clear.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
    clear.DepthStencil = { 1.0f, 0 };

    D3D12_HEAP_PROPERTIES heap(D3D12_HEAP_TYPE_DEFAULT);
    HRESULT hr = DirectXEngine::GetDevice()->CreateCommittedResource(
        &heap, D3D12_HEAP_FLAG_NONE, &desc,
        D3D12_RESOURCE_STATE_DEPTH_WRITE, &clear,
        IID_PPV_ARGS(&shadowMapTexture_));
    assert(SUCCEEDED(hr));

	// シャドウマップ用DSVの作成
    const uint32_t dsvIndex = DsvManager::GetInstance()->Allocate();
    shadowDSV_ = DsvManager::GetInstance()->GetCPUDescriptorHandle(dsvIndex);
    DsvManager::GetInstance()->CreateDSV(dsvIndex, shadowMapTexture_.Get());

	// シャドウマップ用SRVの作成
	shadowSrvIndex_ = SrvManager::GetInstance()->Allocate() + TextureManager::kSRVIndexTop;
    SrvManager::GetInstance()->CreateSRVforDepth(shadowSrvIndex_, shadowMapTexture_.Get(), DXGI_FORMAT_R24_UNORM_X8_TYPELESS);

    shadowViewport_ = { 0.0f, 0.0f, float(size), float(size), 0.0f, 1.0f };
    shadowScissor_ = { 0, 0, LONG(size), LONG(size) };

    shadowState_ = D3D12_RESOURCE_STATE_DEPTH_WRITE;
}

void ShadowMap::BeginShadowMapPass()
{
    auto* commandList = DirectXEngine::GetCommandList();
    // SRV→DSV 書き込み
    TransitionIfNeeded(commandList, D3D12_RESOURCE_STATE_DEPTH_WRITE);

    commandList->RSSetViewports(1, &shadowViewport_);
    commandList->RSSetScissorRects(1, &shadowScissor_);
    // DSV のみ。カラーRTVは設定しない
    commandList->OMSetRenderTargets(0, nullptr, FALSE, &shadowDSV_);
    commandList->ClearDepthStencilView(shadowDSV_, D3D12_CLEAR_FLAG_DEPTH, 1.0f, 0, 0, nullptr);
}

void ShadowMap::EndShadowMapPass()
{
    auto* commandList = DirectXEngine::GetCommandList();
    // DSV→SRV
    TransitionIfNeeded(commandList, D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE);
}

void ShadowMap::TransitionIfNeeded(ID3D12GraphicsCommandList* cmd, D3D12_RESOURCE_STATES newState)
{
    if (!shadowMapTexture_) return;
    if (shadowState_ == newState) return;

    D3D12_RESOURCE_BARRIER b{};
    b.Type = D3D12_RESOURCE_BARRIER_TYPE_TRANSITION;
    b.Flags = D3D12_RESOURCE_BARRIER_FLAG_NONE;
    b.Transition.pResource = shadowMapTexture_.Get();
    b.Transition.Subresource = D3D12_RESOURCE_BARRIER_ALL_SUBRESOURCES;
    b.Transition.StateBefore = shadowState_;
    b.Transition.StateAfter = newState;
    cmd->ResourceBarrier(1, &b);

    shadowState_ = newState;
}
