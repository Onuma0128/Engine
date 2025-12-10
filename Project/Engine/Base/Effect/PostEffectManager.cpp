#include "PostEffectManager.h"

#include <algorithm>

#include "WinApp.h"
#include "DirectXEngine.h"
#include "PipelineState.h"
#include "TextureManager.h"

#include "CameraManager.h"
#include "Camera.h"
#include "SrvManager.h"
#include "RtvManager.h"
#include "DsvManager.h"
#include "RenderTexture.h"
#include "CreateBufferResource.h"

std::unique_ptr<PostEffectManager> PostEffectManager::instance_ = nullptr;

PostEffectManager* PostEffectManager::GetInstance()
{
    if (instance_ == nullptr) {
        instance_ = std::make_unique<PostEffectManager>();
    }
    return instance_.get();
}

void PostEffectManager::Finalize()
{
    enabledEffects_.clear();
    passes_.clear();
    instance_ = nullptr;
}

void PostEffectManager::Initialize(DirectXEngine* dxEngine)
{
	dxEngine_ = dxEngine;
	pipeline_ = dxEngine->GetPipelineState();

    CreateOutLineMaskResource();

    ResourceInitialize();
}

void PostEffectManager::CreatePostEffect(PostEffectType type)
{
    // 既に登録済みならスキップ
    if (passes_.contains(type)) return;

    PostEffectPass pass;

    // RenderTexture作成
    pass.renderTexture = RenderTexture::CreateResource(
        dxEngine_->GetDevice(),
        WinApp::kClientWidth,
        WinApp::kClientHeight,
        DXGI_FORMAT_R8G8B8A8_UNORM_SRGB,
        { 0.0f,0.0f,0.0f,1.0f }
    );
    
    // DSV/RTV/SRV 登録
    pass.rtvIndex = RtvManager::GetInstance()->Allocate();
    RtvManager::GetInstance()->CreateRTV(pass.rtvIndex, pass.renderTexture.Get());

    pass.srvIndex = SrvManager::GetInstance()->Allocate() + TextureManager::kSRVIndexTop;
    SrvManager::GetInstance()->CreateSRVforRenderTexture(pass.srvIndex, pass.renderTexture.Get());

    if (type == PostEffectType::kOutLine) {
        pass.depthSrvIndex = SrvManager::GetInstance()->Allocate() + TextureManager::kSRVIndexTop;
        SrvManager::GetInstance()->CreateSRVforRenderTexture(pass.depthSrvIndex, dxEngine_->GetRenderTexrure()->GetDSVResource(), type);
    }

    // PSOとRootSignatureの登録
    rootSignatures_[type] = pipeline_->GetRootSignature(PipelineType::kRenderTexture, type, BlendMode::kBlendModeNone).Get();
    pipelineStates_[type] = pipeline_->GetPipelineState(PipelineType::kRenderTexture, type, BlendMode::kBlendModeNone).Get();

    // 保存
    enabledEffects_.push_back(type);
    passes_[type] = pass;
}

void PostEffectManager::BeginOutlineMaskPass()
{
    auto* cmd = DirectXEngine::GetCommandList();

    // 1) outlineMask: SRV -> RTV
    D3D12_RESOURCE_BARRIER b{};
    b.Type = D3D12_RESOURCE_BARRIER_TYPE_TRANSITION;
    b.Transition.pResource = maskPass_.outlineMask.Get();
    b.Transition.StateBefore = D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE;
    b.Transition.StateAfter = D3D12_RESOURCE_STATE_RENDER_TARGET;
    b.Transition.Subresource = D3D12_RESOURCE_BARRIER_ALL_SUBRESOURCES;
    cmd->ResourceBarrier(1, &b);

    // 2) objectID: SRV -> RTV
    b.Transition.pResource = maskPass_.objectID.Get();
    b.Transition.StateBefore = D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE;
    b.Transition.StateAfter = D3D12_RESOURCE_STATE_RENDER_TARGET;
    cmd->ResourceBarrier(1, &b);

    // 3) Depth: WRITE -> READ (Maskパスは深度読みのみ)
    b.Transition.pResource = dxEngine_->GetRenderTexrure()->GetDSVResource();
    b.Transition.StateBefore = D3D12_RESOURCE_STATE_DEPTH_WRITE;
    b.Transition.StateAfter = D3D12_RESOURCE_STATE_DEPTH_READ;
    cmd->ResourceBarrier(1, &b);

    // 4) MRT + DSV をバインド
    D3D12_CPU_DESCRIPTOR_HANDLE rtvs[2] = {
        RtvManager::GetInstance()->GetCPUDescriptorHandle(maskPass_.rtvIndex),    // Mask
        RtvManager::GetInstance()->GetCPUDescriptorHandle(maskPass_.rtvIndexID)   // ObjectID
    };
    auto dsv = DsvManager::GetInstance()->GetCPUDescriptorHandle(maskPass_.depthDsvIndex);
    cmd->OMSetRenderTargets(2, rtvs, FALSE, &dsv);

    // 5) クリア（Mask=黒、ID=0）
    const FLOAT clearMask[4] = { 0,0,0,0 };
    cmd->ClearRenderTargetView(rtvs[0], clearMask, 0, nullptr);
    const FLOAT clearID[4] = { 0,0,0,0 }; // 整数RTでも0ならOK
    cmd->ClearRenderTargetView(rtvs[1], clearID, 0, nullptr);
}

void PostEffectManager::EndOutlineMaskPass()
{
    auto* cmd = DirectXEngine::GetCommandList();
    D3D12_RESOURCE_BARRIER b{};

    // Mask: RTV -> SRV
    b.Type = D3D12_RESOURCE_BARRIER_TYPE_TRANSITION;
    b.Transition.pResource = maskPass_.outlineMask.Get();
    b.Transition.StateBefore = D3D12_RESOURCE_STATE_RENDER_TARGET;
    b.Transition.StateAfter = D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE;
    b.Transition.Subresource = D3D12_RESOURCE_BARRIER_ALL_SUBRESOURCES;
    cmd->ResourceBarrier(1, &b);

    // ObjectID: RTV -> SRV
    b.Transition.pResource = maskPass_.objectID.Get();
    cmd->ResourceBarrier(1, &b);

    // Depth: READ -> WRITE（元に戻す）
    b.Transition.pResource = dxEngine_->GetRenderTexrure()->GetDSVResource();
    b.Transition.StateBefore = D3D12_RESOURCE_STATE_DEPTH_READ;
    b.Transition.StateAfter = D3D12_RESOURCE_STATE_DEPTH_WRITE;
    cmd->ResourceBarrier(1, &b);
}

void PostEffectManager::RenderTextureDraws(uint32_t inputSRVIndex)
{
    Update();

    auto* cmdList = dxEngine_->GetCommandList();
    uint32_t finalSRVIndex = inputSRVIndex;

    for (PostEffectType type : enabledEffects_) {
        const auto& pass = passes_[type];

        D3D12_RESOURCE_BARRIER barrier{};
        if (type == PostEffectType::kOutLine) {
            barrier.Type = D3D12_RESOURCE_BARRIER_TYPE_TRANSITION;
            barrier.Flags = D3D12_RESOURCE_BARRIER_FLAG_NONE;
            barrier.Transition.pResource = dxEngine_->GetRenderTexrure()->GetDSVResource();
            barrier.Transition.StateBefore = D3D12_RESOURCE_STATE_DEPTH_WRITE;
            barrier.Transition.StateAfter = D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE;
            cmdList->ResourceBarrier(1, &barrier);
        }

        // RTVへ遷移
        barrier.Transition.pResource = pass.renderTexture.Get();
        barrier.Transition.StateBefore = D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE;
        barrier.Transition.StateAfter = D3D12_RESOURCE_STATE_RENDER_TARGET;
        cmdList->ResourceBarrier(1, &barrier);

        D3D12_CPU_DESCRIPTOR_HANDLE rtvHandle = RtvManager::GetInstance()->GetCPUDescriptorHandle(pass.rtvIndex);
        cmdList->OMSetRenderTargets(1, &rtvHandle, FALSE, nullptr);

        cmdList->SetGraphicsRootSignature(rootSignatures_[type].Get());
        cmdList->SetPipelineState(pipelineStates_[type].Get());
        cmdList->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
        SrvManager::GetInstance()->SetGraphicsRootDescriptorTable(0, finalSRVIndex);
        if (type == PostEffectType::kOutLine) { 
            SrvManager::GetInstance()->SetGraphicsRootDescriptorTable(1, pass.depthSrvIndex);
            SrvManager::GetInstance()->SetGraphicsRootDescriptorTable(2, maskPass_.srvIndex);
            SrvManager::GetInstance()->SetGraphicsRootDescriptorTable(3, maskPass_.srvIndexID);
        }
        PostEffectCommand(type);
        cmdList->DrawInstanced(3, 1, 0, 0);

        // 次のエフェクト用にSRVとして使えるように遷移
        barrier.Transition.pResource = pass.renderTexture.Get();
        barrier.Transition.StateBefore = D3D12_RESOURCE_STATE_RENDER_TARGET;
        barrier.Transition.StateAfter = D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE;
        cmdList->ResourceBarrier(1, &barrier);

        if (type == PostEffectType::kOutLine)
        {
            barrier.Transition.pResource = dxEngine_->GetRenderTexrure()->GetDSVResource();
            barrier.Transition.StateBefore = D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE;
            barrier.Transition.StateAfter = D3D12_RESOURCE_STATE_DEPTH_WRITE;
            cmdList->ResourceBarrier(1, &barrier);
        }

        // 次のエフェクトの入力として更新
        finalSRVIndex = pass.srvIndex;
    }

    // 最終的なSRVIndexをSwapChain描画に使う
    finalSRVIndex_ = finalSRVIndex;
}

void PostEffectManager::PostEffectCommand(PostEffectType type)
{
    auto* cmdList = dxEngine_->GetCommandList();

    switch (type)
    {
    case PostEffectType::kNone:
        break;
    case PostEffectType::kGrayscale:

        cmdList->SetGraphicsRootConstantBufferView(1, grayscaleResource_->GetGPUVirtualAddress());

        break;
    case PostEffectType::kVignette:

        cmdList->SetGraphicsRootConstantBufferView(1, vignetteResource_->GetGPUVirtualAddress());

        break;
    case PostEffectType::kSmoothing:

        break;

    case PostEffectType::kOutLine:

        cmdList->SetGraphicsRootConstantBufferView(4, outlineResource_->GetGPUVirtualAddress());

        break;
    default:
        break;
    }
}

uint32_t PostEffectManager::DrawEffect(PostEffectType type, uint32_t inputSRVIndex)
{
    // 未作成なら実行不可
    if (!passes_.contains(type)) { return inputSRVIndex; }

    auto* cmdList = dxEngine_->GetCommandList();
    const auto& pass = passes_[type];

    D3D12_RESOURCE_BARRIER barrier{};
    barrier.Type = D3D12_RESOURCE_BARRIER_TYPE_TRANSITION;
    barrier.Flags = D3D12_RESOURCE_BARRIER_FLAG_NONE;

    // OutLineだけは深度をPS用にREADへ（実行前）
    if (type == PostEffectType::kOutLine) {
        barrier.Transition.pResource = dxEngine_->GetRenderTexrure()->GetDSVResource();
        barrier.Transition.StateBefore = D3D12_RESOURCE_STATE_DEPTH_WRITE;
        barrier.Transition.StateAfter = D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE;
        cmdList->ResourceBarrier(1, &barrier);
    }

    // このパスのRTVへ
    barrier.Transition.pResource = pass.renderTexture.Get();
    barrier.Transition.StateBefore = D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE;
    barrier.Transition.StateAfter = D3D12_RESOURCE_STATE_RENDER_TARGET;
    cmdList->ResourceBarrier(1, &barrier);

    // RTVバインド
    auto rtv = RtvManager::GetInstance()->GetCPUDescriptorHandle(pass.rtvIndex);
    cmdList->OMSetRenderTargets(1, &rtv, FALSE, nullptr);

    // PSO / RS セット
    cmdList->SetGraphicsRootSignature(rootSignatures_[type].Get());
    cmdList->SetPipelineState(pipelineStates_[type].Get());
    cmdList->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

    // 入力テクスチャ
    SrvManager::GetInstance()->SetGraphicsRootDescriptorTable(0, inputSRVIndex);

    // OutLineだけ追加SRV（深度 / マスク2種）
    if (type == PostEffectType::kOutLine) {
        SrvManager::GetInstance()->SetGraphicsRootDescriptorTable(1, pass.depthSrvIndex);
        SrvManager::GetInstance()->SetGraphicsRootDescriptorTable(2, maskPass_.srvIndex);
        SrvManager::GetInstance()->SetGraphicsRootDescriptorTable(3, maskPass_.srvIndexID);
    }

    // パラメータCBVなど
    PostEffectCommand(type);

    // 描画
    cmdList->DrawInstanced(3, 1, 0, 0);

    // 出力をSRVへ戻す
    barrier.Transition.pResource = pass.renderTexture.Get();
    barrier.Transition.StateBefore = D3D12_RESOURCE_STATE_RENDER_TARGET;
    barrier.Transition.StateAfter = D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE;
    cmdList->ResourceBarrier(1, &barrier);

    // OutLineだけ深度をWRITEに戻す（実行後）
    if (type == PostEffectType::kOutLine) {
        barrier.Transition.pResource = dxEngine_->GetRenderTexrure()->GetDSVResource();
        barrier.Transition.StateBefore = D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE;
        barrier.Transition.StateAfter = D3D12_RESOURCE_STATE_DEPTH_WRITE;
        cmdList->ResourceBarrier(1, &barrier);
    }

    // このパスの出力SRVを返す（次の入力になる）
    return pass.srvIndex;
}

void PostEffectManager::ResourceInitialize()
{
    grayscaleResource_ = CreateBufferResource(DirectXEngine::GetDevice(), sizeof(GrayscaleData));
    grayscaleResource_->Map(0, nullptr, reinterpret_cast<void**>(&grayscaleData_));
    grayscaleData_->color = { 0.2125f, 0.7154f, 0.0721f };
    grayscaleData_->t = 0.0f;

    vignetteResource_ = CreateBufferResource(DirectXEngine::GetDevice(), sizeof(VignetteData));
    vignetteResource_->Map(0, nullptr, reinterpret_cast<void**>(&vignetteData_));
    vignetteData_->scale = 16.0f;
    vignetteData_->gamma = 0.0f;

    outlineResource_ = CreateBufferResource(DirectXEngine::GetDevice(), sizeof(OutlineData));
    outlineResource_->Map(0, nullptr, reinterpret_cast<void**>(&outlineData_));
    outlineData_->projection = Matrix4x4::Identity();
}

void PostEffectManager::CreateOutLineMaskResource()
{
    // RenderTexture作成
    maskPass_.outlineMask = RenderTexture::CreateResource(
        dxEngine_->GetDevice(),
        WinApp::kClientWidth,
        WinApp::kClientHeight,
        DXGI_FORMAT_R8G8B8A8_UNORM,
        { 0.0f,0.0f,0.0f,0.0f }
    );
    maskPass_.objectID = RenderTexture::CreateResource(
        dxEngine_->GetDevice(),
        WinApp::kClientWidth,
        WinApp::kClientHeight,
        DXGI_FORMAT_R32_UINT,
        { 0.0f,0.0f,0.0f,0.0f }
    );

    // DSV/RTV/SRV 登録
    maskPass_.rtvIndex = RtvManager::GetInstance()->Allocate();
    RtvManager::GetInstance()->CreateRTV(
        maskPass_.rtvIndex, 
        maskPass_.outlineMask.Get(),
        DXGI_FORMAT_R8G8B8A8_UNORM
    );
    maskPass_.rtvIndexID = RtvManager::GetInstance()->Allocate();
    RtvManager::GetInstance()->CreateRTV(
        maskPass_.rtvIndexID,
        maskPass_.objectID.Get(),
        DXGI_FORMAT_R32_UINT
    );

    maskPass_.depthDsvIndex = DsvManager::GetInstance()->Allocate();
    DsvManager::GetInstance()->CreateOutlineDSV(
        maskPass_.depthDsvIndex, 
        dxEngine_->GetRenderTexrure()->GetDSVResource()
    );

    maskPass_.srvIndex = SrvManager::GetInstance()->Allocate() + TextureManager::kSRVIndexTop;
    SrvManager::GetInstance()->CreateSRVforTexture2D(
        maskPass_.srvIndex, maskPass_.outlineMask.Get(), DXGI_FORMAT_R8G8B8A8_UNORM, 1);

    maskPass_.srvIndexID = SrvManager::GetInstance()->Allocate() + TextureManager::kSRVIndexTop;
    SrvManager::GetInstance()->CreateSRVforTexture2D(
        maskPass_.srvIndexID, maskPass_.objectID.Get(), DXGI_FORMAT_R32_UINT, 1);
}

void PostEffectManager::Update()
{
    grayscaleData_->t = std::clamp(grayscaleData_->t, 0.0f, 2.0f);

    outlineData_->projection = Matrix4x4::Inverse(CameraManager::GetInstance()->GetActiveCamera()->GetProjectionMatrix());
}
