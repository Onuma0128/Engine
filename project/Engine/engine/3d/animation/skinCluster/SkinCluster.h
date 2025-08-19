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

class SkinCluster
{
private:

	static const uint32_t kNumMaxInfluence = 4;
	struct VertexInfluence {
		std::array<float, kNumMaxInfluence> weights;
		std::array<uint32_t, kNumMaxInfluence> jointIndices;
	};

	struct SkinClusterItem {
		std::vector<Matrix4x4> inverseBindPoseMatrices;
		ComPtr<ID3D12Resource> infuenceResource;
		D3D12_VERTEX_BUFFER_VIEW influenceBufferView;
		std::span<VertexInfluence> mappedInfluence;
		std::vector<WellForGPU> mappedPalettes;
	};

public:

	void Reset();

	void CreateSkinCluster(Skeleton& skeleton, const ModelData& modelData);

	void SkinClusterUpdate(Skeleton& skeleton);


	const std::vector<WellForGPU>& GetWellForGPU()const { return item_.mappedPalettes; }
	const D3D12_VERTEX_BUFFER_VIEW& GetInfluenceBufferView()const { return item_.influenceBufferView; }

private:

	SkinClusterItem item_;

};

