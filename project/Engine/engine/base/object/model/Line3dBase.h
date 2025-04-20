#pragma once
#include <d3d12.h>
#pragma comment(lib,"d3d12.lib")
#include "wrl.h"

using Microsoft::WRL::ComPtr;

class DirectXEngine;

class Line3dBase
{
private:
	static Line3dBase* instance_;

	Line3dBase() = default;
	~Line3dBase() = default;
	Line3dBase(Line3dBase&) = delete;
	Line3dBase& operator=(Line3dBase&) = delete;

public:
	// シングルトンインスタンスの取得
	static Line3dBase* GetInstance();

	void Initialize(DirectXEngine* dxEngine);

	void DrawBase();

	void Finalize();

	/*==================== アクセッサ ====================*/

	DirectXEngine* GetDxEngine() const { return dxEngine_; }

private:

	DirectXEngine* dxEngine_ = nullptr;

	ComPtr<ID3D12RootSignature> rootSignature_ = nullptr;
	ComPtr<ID3D12PipelineState> pipelineState_ = nullptr;
};