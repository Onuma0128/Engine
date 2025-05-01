#pragma once
#include <d3d12.h>
#pragma comment(lib,"d3d12.lib")
#include "wrl.h"

#include <array>

using Microsoft::WRL::ComPtr;

class DirectXEngine;
class PipelineState;

class PrimitiveDrawrBase
{
private:

	static PrimitiveDrawrBase* instance_;

	PrimitiveDrawrBase() = default;
	~PrimitiveDrawrBase() = default;
	PrimitiveDrawrBase(PrimitiveDrawrBase&) = delete;
	PrimitiveDrawrBase& operator=(PrimitiveDrawrBase&) = delete;

public:

	static PrimitiveDrawrBase* GetInstance();

	void Initialize(DirectXEngine* dxEngine);

	void DrawBase(int blendMode);

	void DrawSkyboxBase();

	void Finalize();

	/*==================== アクセッサ ====================*/

	DirectXEngine* GetDxEngine() const { return dxEngine_; }

private:

	DirectXEngine* dxEngine_ = nullptr;

	ComPtr<ID3D12RootSignature> rootSignature_ = nullptr;
	std::array<ComPtr<ID3D12PipelineState>, 5> pipelineStates_;

	ComPtr<ID3D12RootSignature> skyboxRootSignature_ = nullptr;
	ComPtr<ID3D12PipelineState> skyboxPipelineState_;

};