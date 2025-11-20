#pragma once

#include <vector>
#include <array>
#include <span>

#include <d3d12.h>
#pragma comment(lib,"d3d12.lib")
#include <wrl.h>
using Microsoft::WRL::ComPtr;

#include "Matrix4x4.h"
#include "ModelStruct.h"
#include "AnimationStruct.h"

class Skeleton;

/// <summary>
/// スキンのクラスタリングクラス
/// </summary>
class SkinCluster
{
private:

	/// <summary>
	/// 最大影響数
	/// </summary>
	static const uint32_t kNumMaxInfluence = 4;

	/// <summary>
	/// 頂点の影響情報
	/// </summary>
	struct VertexInfluence {
		std::array<float, kNumMaxInfluence> weights;
		std::array<uint32_t, kNumMaxInfluence> jointIndices;
	};

	/// <summary>
	/// スキンのクラスタリング項目
	/// </summary>
	struct SkinClusterItem {
		std::vector<Matrix4x4> inverseBindPoseMatrices;
		ComPtr<ID3D12Resource> infuenceResource;
		D3D12_VERTEX_BUFFER_VIEW influenceBufferView;
		std::span<VertexInfluence> mappedInfluence;
		std::vector<WellForGPU> mappedPalettes;
	};

public:

	/// <summary>
	/// スキンのクラスタリング項目をリセット
	/// </summary>
	void Reset();

	/// <summary>
	/// スキンのクラスタリング項目を作成
	/// </summary>
	/// <param name="skeleton"></param>
	/// <param name="modelData"></param>
	void CreateSkinCluster(Skeleton& skeleton, const ModelData& modelData);

	/// <summary>
	/// スキンのクラスタリング項目を更新
	/// </summary>
	/// <param name="skeleton"></param>
	void SkinClusterUpdate(Skeleton& skeleton);

	/// <summary>
	/// アクセッサ
	/// </summary>
	
	// 逆バインドポーズ行列群を取得
	const std::vector<WellForGPU>& GetWellForGPU()const { return item_.mappedPalettes; }
	// 影響バッファビューを取得
	const D3D12_VERTEX_BUFFER_VIEW& GetInfluenceBufferView()const { return item_.influenceBufferView; }

private:

	SkinClusterItem item_;

};

