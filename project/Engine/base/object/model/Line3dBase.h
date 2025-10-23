#pragma once
#include <d3d12.h>
#pragma comment(lib,"d3d12.lib")
#include "wrl.h"

using Microsoft::WRL::ComPtr;

/// <summary>
/// ラインのPSOを管理するクラス
/// </summary>
class Line3dBase
{
public:

	// 初期化
	void Initialize();
	// 描画
	void DrawBase();

private:

	ComPtr<ID3D12RootSignature> rootSignature_ = nullptr;
	ComPtr<ID3D12PipelineState> pipelineState_ = nullptr;
};