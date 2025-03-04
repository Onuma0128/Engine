#pragma once
#include <d3d12.h>
#pragma comment(lib,"d3d12.lib")
#include "wrl.h"

using Microsoft::WRL::ComPtr;

class DirectXEngine;

class AnimationBase
{
private:
	static AnimationBase* instance_;

	AnimationBase() = default;
	~AnimationBase() = default;
	AnimationBase(AnimationBase&) = delete;
	AnimationBase& operator=(AnimationBase&) = delete;

public:
	// シングルトンインスタンスの取得
	static AnimationBase* GetInstance();

	void Initialize(DirectXEngine* dxEngine);

	void DrawBase();

	void Finalize();


	// ゲッター
	DirectXEngine* GetDxEngine() const { return dxEngine_; }

private:

	DirectXEngine* dxEngine_ = nullptr;

	// ルートシグネチャ
	ComPtr<ID3D12RootSignature> rootSignature_ = nullptr;
	// パイプラインステート
	ComPtr<ID3D12PipelineState> pipelineState_ = nullptr;
};

