#pragma once
#include <d3d12.h>
#pragma comment(lib,"d3d12.lib")
#include "wrl.h"

#include <stdint.h>

using Microsoft::WRL::ComPtr;

class DirectXEngine;

/// <summary>
/// RTV管理クラス
/// </summary>
class RtvManager
{
private:

	// シングルトンインスタンス
	static RtvManager* instance_;

	RtvManager() = default;
	~RtvManager() = default;
	RtvManager(const RtvManager&) = delete;
	RtvManager& operator=(const RtvManager&) = delete;

public:

	// 最大RTV数
	static const uint32_t kMaxRTVCount = 256;
	// シングルトンインスタンスの取得
	static RtvManager* GetInstance();
	// 初期化
	void Initialize(DirectXEngine* dxEngine);
	// 終了処理
	void Finalize();

	// ハンドル取得
	D3D12_CPU_DESCRIPTOR_HANDLE GetCPUDescriptorHandle(uint32_t index);

	// RTV作成
	void CreateRTV(uint32_t index, ID3D12Resource* resource,DXGI_FORMAT format = DXGI_FORMAT_R8G8B8A8_UNORM_SRGB);

	// 自動割当
	uint32_t Allocate();

private:
	DirectXEngine* dxEngine_ = nullptr;

	ComPtr<ID3D12DescriptorHeap> descriptorHeap_;
	uint32_t descriptorSize_ = 0;

	uint32_t useIndex_ = 0;
};
