#pragma once
#include <d3d12.h>
#pragma comment(lib,"d3d12.lib")
#include "wrl.h"

#include <stdint.h>

using Microsoft::WRL::ComPtr;

class DirectXEngine;

/// <summary>
/// DSV管理クラス
/// </summary>
class DsvManager
{
private:

	// シングルトンインスタンス
	static DsvManager* instance_;

	DsvManager() = default;
	~DsvManager() = default;
	DsvManager(const DsvManager&) = delete;
	DsvManager& operator=(const DsvManager&) = delete;

public:

	// 最大DSV数
	static const uint32_t kMaxDSVCount = 256;
	// シングルトンインスタンスの取得
	static DsvManager* GetInstance();
	// 初期化
	void Initialize(DirectXEngine* dxEngine);
	// 終了処理
	void Finalize();

	// ハンドル取得
	D3D12_CPU_DESCRIPTOR_HANDLE GetCPUDescriptorHandle(uint32_t index);

	// DSV作成
	void CreateDSV(uint32_t index, ID3D12Resource* resource);
	void CreateOutlineDSV(uint32_t index, ID3D12Resource* resource);

	// 自動割当
	uint32_t Allocate();

private:
	DirectXEngine* dxEngine_ = nullptr;

	ComPtr<ID3D12DescriptorHeap> descriptorHeap_;
	uint32_t descriptorSize_ = 0;
	uint32_t useIndex_ = 0;
};

