#pragma once
#include <d3d12.h>
#pragma comment(lib,"d3d12.lib")
#include "wrl.h"

#include <array>

using Microsoft::WRL::ComPtr;

/// <summary>
/// プリミティブ描画のPSOを管理するクラス
/// </summary>
class PrimitiveDrawrBase
{
public:

	// 初期化
	void Initialize();
	// 描画
	void DrawBase(int blendMode);
	// スカイボックス描画
	void DrawSkyboxBase();

private:

	ComPtr<ID3D12RootSignature> rootSignature_ = nullptr;
	std::array<ComPtr<ID3D12PipelineState>, 5> pipelineStates_;

	ComPtr<ID3D12RootSignature> skyboxRootSignature_ = nullptr;
	ComPtr<ID3D12PipelineState> skyboxPipelineState_;

};