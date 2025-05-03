#pragma once
#include <d3d12.h>
#include <dxgi1_6.h>
#include <wrl.h>

#include <dxcapi.h>
#include <array>
#include <chrono>

#include "StringUtility.h"
#include "PipelineState.h"

class WinApp;
class ImGuiManager;

using Microsoft::WRL::ComPtr;

class DirectXEngine
{
public:

	~DirectXEngine();

	// 初期化
	void Initialize(WinApp* winApp, ImGuiManager* imguiManager);
	// デバイスの初期化
	void DeviceInitialize();
	// コマンド関連の初期化
	void CommandInitialize();
	// スワップチェーンの初期化
	void SwapChainInitialize();
	// 深度バッファの初期化
	void DepthStencilInitialize();
	// 各種デスクリプタヒープの初期化
	void DescriptorHeapInitialize();
	// レンダーターゲットビューの初期化
	void RTVInitialize();
	// フェンスの初期化
	void FenceInitialize();
	// ビューポートの初期化
	void ViewportInitialize();
	// シザー矩形の初期化
	void RectInitialize();
	// DXCコンパイラの初期化
	void DxcCompilerInitialize();
	// IncludeHandlerの初期化
	void IncludeHandlerInitialize();
	// PipelineStateの初期化
	void PipelineStateInitialize();
	// FPS固定の初期化
	void InitializeFixFPS();
	// FPS固定の更新
	void UpdateFixFPS();

	// 描画前の処理
	void PreDraw();
	// Object描画後の処理
	void SwapChainDrawSet();
	// 描画後の処理
	void PostDraw();

	/*========================== ゲッター ===========================*/

	// デバイス
	ID3D12Device* GetDevice()const { return device_.Get(); }
	// コマンドリスト
	ID3D12GraphicsCommandList* GetCommandList()const { return commandList_.Get(); }
	// パイプラインのゲッター
	PipelineState* GetPipelineState()const { return pipelineState_.get(); }
	// バックバッファの数を取得
	size_t GetBackBufferCount()const { return static_cast<size_t>(swapChainDesc_.BufferCount); }

private:
	// StringUtility
	std::unique_ptr<StringUtility> stringUtility_ = nullptr;
	// WindowsAPI
	WinApp* winApp_ = nullptr;
	// PipelineState
	std::unique_ptr<PipelineState> pipelineState_ = nullptr;

	///==============================================================

	// D3D12Deviceの作成
	ComPtr<ID3D12Device> device_ = nullptr;
	// DXGIファクトリーの生成
	ComPtr<IDXGIFactory7> dxgiFactory_ = nullptr;
	//コマンドキューを生成する
	ComPtr<ID3D12CommandQueue> commandQueue_ = nullptr;
	//コマンドリストを生成する
	ComPtr<ID3D12GraphicsCommandList> commandList_ = nullptr;
	//コマンドアロケータを生成する
	ComPtr<ID3D12CommandAllocator> commandAllocator_ = nullptr;
	//スワップチェーンを生成する
	ComPtr<IDXGISwapChain4> swapChain_ = nullptr;
	DXGI_SWAP_CHAIN_DESC1 swapChainDesc_{};
	std::array<ComPtr<ID3D12Resource>, 2> swapChainResources_ = { nullptr };
	// レンダーテクスチャを生成する
	uint32_t renderTextureSRVIndex_;
	ComPtr<ID3D12Resource> renderTextureResource_ = nullptr;
	D3D12_CPU_DESCRIPTOR_HANDLE renderTextureHandle_{};
	// ルートシグネチャ,パイプラインステート
	ComPtr<ID3D12RootSignature> offScreenRootSignature_ = nullptr;
	ComPtr<ID3D12PipelineState> offScreenPipelineState_ = nullptr;
	// 深度バッファの生成
	ComPtr<ID3D12Resource> depthStencilResource_ = nullptr;
	ComPtr<ID3D12DescriptorHeap> dsvDescriptorHeap_ = nullptr;
	//RTVを2つ作るのでディスクリプタを2つ用意
	std::array<D3D12_CPU_DESCRIPTOR_HANDLE, 2> rtvHandles_{};
	// フェンスを生成
	ComPtr<ID3D12Fence> fence_ = nullptr;
	uint64_t fenceValue_ = 0;
	HANDLE fenceEvent_{};
	// ビューポートの生成
	D3D12_VIEWPORT viewport_{};
	// シザー矩形の生成
	D3D12_RECT scissorRect_{};
	// DxcCompilerの生成
	ComPtr<IDxcUtils> dxcUtils_ = nullptr;
	ComPtr<IDxcCompiler3> dxcCompiler_ = nullptr;
	// IncludeHandlerの生成
	ComPtr<IDxcIncludeHandler> includeHandler_ = nullptr;
	// TransitionBarrierの生成
	D3D12_RESOURCE_BARRIER barrier_{};
	// 記録時間
	std::chrono::steady_clock::time_point reference_;
};