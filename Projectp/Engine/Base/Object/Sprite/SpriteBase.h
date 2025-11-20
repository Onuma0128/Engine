#pragma once
#include <d3d12.h>
#pragma comment(lib,"d3d12.lib")
#include "wrl.h"

using Microsoft::WRL::ComPtr;

/// <summary>
/// アニメーションのPSOを管理するクラス
/// </summary>
class SpriteBase
{
public:

	// 初期化
	void Initialize();
	// 描画
	void DrawBase();

private:

	// ルートシグネチャ
	ComPtr<ID3D12RootSignature> rootSignature_ = nullptr;
	// パイプラインステート
	ComPtr<ID3D12PipelineState> pipelineState_ = nullptr;
};